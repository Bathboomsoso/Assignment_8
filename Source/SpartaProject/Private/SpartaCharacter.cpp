#include "SpartaCharacter.h"
#include "EnhancedInputComponent.h"
#include "SpartaGameState.h"
#include "SpartaHUD.h"
#include "SpartaPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/Image.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	NormalMoveSpeed = 600.0f;
	SprintMMoveSpeed = 2.00f;
	SprintMoveSpeed = NormalMoveSpeed * SprintMMoveSpeed;

	GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;

	MaxHealth = 100.0f;
	Health = MaxHealth;

	bIsDebuff = false;

	UpdateOverheadSpeed();
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Move
					);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::Look
					);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartJump
					);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopJump
					);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaCharacter::StartSprint
					);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&ASpartaCharacter::StopSprint
					);
			}
		}
	}
}

void ASpartaCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector MoveInput = value.Get<FVector>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
		UpdateOverheadSpeed();
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
		UpdateOverheadSpeed();
	}
}

void ASpartaCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASpartaCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASpartaCharacter::Look(const FInputActionValue& value)
{
	FVector LookInput = value.Get<FVector>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint()
{
	if (GetCharacterMovement())
	{
		if (bIsDebuff) return;
		GetCharacterMovement()->MaxWalkSpeed = SprintMoveSpeed;
	}
}

void ASpartaCharacter::StopSprint()
{
	if (GetCharacterMovement())
	{
		if (bIsDebuff) return;
		GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;
	}
}

void ASpartaCharacter::SlowSpeed(float DebuffSpeed)
{
	bIsDebuff = true;
	GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed - DebuffSpeed;
	UpdateOverheadSpeed();
	SlowDebuffIcon();
}

void ASpartaCharacter::FastSpeed(float BoostSpeed)
{
	bIsDebuff = false;
	GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed + BoostSpeed;
	UpdateOverheadSpeed();
	ActiveBlind();
	SlowDebuffIcon();
}

float ASpartaCharacter::GetHealth() const
{
	return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

float ASpartaCharacter::TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if (Health <= 0.0f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASpartaCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;
	
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;
	
	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}

void ASpartaCharacter::UpdateOverheadSpeed()
{
	if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(GetController()))
	{
		if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
		{
			if (UTextBlock* SpeedText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("SpeedValue"))))
			{
				SpeedText->SetText(FText::FromString(FString::Printf(TEXT("Speed : %.f"), GetCharacterMovement()->MaxWalkSpeed)));
			}
		}
	}
}

float ASpartaCharacter::GetHealthPercentage() const
{
	if (MaxHealth <= 0)
	{
		return 0.0f;
	}
	return (Health / MaxHealth);
}

void ASpartaCharacter::ActiveBlind()
{
	if (bIsDebuff)
	{
		ASpartaPlayerController* PC = Cast<ASpartaPlayerController>(GetController());
		if (!PC) return;
		UUserWidget* HUD = Cast<UUserWidget>(PC->GetHUDWidget());
		if (!HUD) return;
		if (UImage* IG = Cast<UImage>(HUD->GetWidgetFromName(TEXT("WBP_Blind"))))
		{
			UFunction* PlayAnimFunc = HUD->FindFunction(FName("Blind"));
			if (PlayAnimFunc)
			{
				HUD->ProcessEvent(PlayAnimFunc, nullptr);
			}
		}
	}
	else
	{
		ASpartaPlayerController* PC = Cast<ASpartaPlayerController>(GetController());
		if (!PC) return;
		UUserWidget* HUD = Cast<UUserWidget>(PC->GetHUDWidget());
		if (!HUD) return;
		if (UImage* IG = Cast<UImage>(HUD->GetWidgetFromName(TEXT("WBP_Blind"))))
		{
			UFunction* Stop = HUD->FindFunction(FName("StopBlind"));
			if (Stop)
			{
				HUD->ProcessEvent(Stop, nullptr);
			}
		}
	}
	
}

void ASpartaCharacter::SlowDebuffIcon()
{
	if (bIsDebuff)
	{
		ASpartaPlayerController* PC = Cast<ASpartaPlayerController>(GetController());
		if (!PC) return;
		UUserWidget* HUD = Cast<UUserWidget>(PC->GetHUDWidget());
		if (!HUD) return;
		if (UImage* IG = Cast<UImage>(HUD->GetWidgetFromName(TEXT("SlowDebuff"))))
		{
			IG->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		ASpartaPlayerController* PC = Cast<ASpartaPlayerController>(GetController());
		if (!PC) return;
		UUserWidget* HUD = Cast<UUserWidget>(PC->GetHUDWidget());
		if (!HUD) return;
		if (UImage* IG = Cast<UImage>(HUD->GetWidgetFromName(TEXT("SlowDebuff"))))
		{
			IG->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}