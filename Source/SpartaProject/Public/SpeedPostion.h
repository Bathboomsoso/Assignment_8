#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SpeedPostion.generated.h"

UCLASS()
class SPARTAPROJECT_API ASpeedPostion : public ABaseItem
{
	GENERATED_BODY()

public:
	ASpeedPostion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 SpeedAmount;

	virtual void ActivateItem(AActor* Activator) override;
};