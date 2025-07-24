#include "SpeedPostion.h"
#include "SpartaCharacter.h"

ASpeedPostion::ASpeedPostion()
{
	SpeedAmount = 150.0f;
	ItemType = "Fasting";
}
void ASpeedPostion::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator-> ActorHasTag("Player"))
	{
		if(ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->FastSpeed(SpeedAmount);
		}
		DestroyedItem();
	}
}


