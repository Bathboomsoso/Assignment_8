#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowPostion.generated.h"

UCLASS()
class SPARTAPROJECT_API ASlowPostion : public ABaseItem
{
	GENERATED_BODY()

public:
	ASlowPostion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 SlowAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
