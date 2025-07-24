#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SpartaHUD.generated.h"

UCLASS()
class SPARTAPROJECT_API ASpartaHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ActiveBlindWidget();
};
