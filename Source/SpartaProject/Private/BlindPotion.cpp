#include "BlindPotion.h"
#include "SpartaCharacter.h"
#include "SpartaGameState.h"

class ASpartaGameState;

ABlindPotion::ABlindPotion()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABlindPotion::BeginPlay()
{
	Super::BeginPlay();
}

void ABlindPotion::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator-> ActorHasTag("Player"))
	{
		if(ASpartaCharacter* SpartaGameState = Cast<ASpartaCharacter>(Activator))
		{
			SpartaGameState->bIsDebuff = true;
			SpartaGameState->ActiveBlind();
		}
		DestroyedItem();
	}
}


