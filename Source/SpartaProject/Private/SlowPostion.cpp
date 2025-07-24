#include "SlowPostion.h"
#include "SpartaCharacter.h"

ASlowPostion::ASlowPostion()
{
	SlowAmount = 150.0f;
	ItemType = "Slowing";
}
void ASlowPostion::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator-> ActorHasTag("Player"))
	{
		if(ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->SlowSpeed(SlowAmount);
		}
		DestroyedItem();
	}
}


