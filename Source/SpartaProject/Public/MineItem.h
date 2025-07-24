#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

UCLASS()
class SPARTAPROJECT_API AMineItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AMineItem();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;

	FTimerHandle ExplosionTimerHandle;

	bool bHasExloded;

	virtual void ActivateItem(AActor* Activator) override;
	
	virtual void Explode();
};
