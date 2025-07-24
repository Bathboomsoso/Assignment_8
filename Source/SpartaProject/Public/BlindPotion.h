#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.h"
#include "BlindPotion.generated.h"

UCLASS()
class SPARTAPROJECT_API ABlindPotion : public ABaseItem
{
	GENERATED_BODY()

public:	
	ABlindPotion();

	void ActiveBlind();

protected:
	virtual void BeginPlay() override;
	
	UUserWidget* BlindWidget;

	virtual void ActivateItem(AActor* Activator) override;
public:	

};
