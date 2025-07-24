#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

UCLASS()
class SPARTAPROJECT_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASpartaGameState();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Level")
	float LevelDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	void OnLevelTimeUp();
	void StartLevel();
	void EndLevel();
	void OnCoinCollected();
	void UpdateHUD();
};
