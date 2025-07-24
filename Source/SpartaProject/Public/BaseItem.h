// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPROJECT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* PickupParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;
	
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override; // 순수 가상함수
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	virtual void DestroyedItem();
};
