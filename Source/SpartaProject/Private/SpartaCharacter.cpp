#include "SpartaCharacter.h"
#include "EnhancedInputComponent.h"
#include "SpartaGameState.h"
#include "SpartaPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
		
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

void ASpartaCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintMoveSpeed;
	}
}

void ASpartaCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;
	}
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



