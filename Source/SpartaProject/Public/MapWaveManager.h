#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "WaveData.h"
#include "MapWaveManager.generated.h"

UCLASS()
class SPARTAPROJECT_API AMapWaveManager : public AActor 
{
    GENERATED_BODY()

public:
    AMapWaveManager();

    UFUNCTION(BlueprintCallable, Category = "Wave Management")
    void StartWave(int32 WaveNumber);

protected:
    UPROPERTY(EditAnywhere, Category = "Wave Settings")
    class UDataTable* WaveDataTable;

    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    FVector2D MapBoundsMin = FVector2D(-5000.0f, -5000.0f);
    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    FVector2D MapBoundsMax = FVector2D(5000.0f, 5000.0f);
    
    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    float SpawnZLocation = 100.0f;
    UPROPERTY(EditAnywhere, Category = "Spawn Settings")
    int32 MaxSpawnAttempts = 100;

private:
    void StartWaveInternal(const FWaveData& WaveData);
    void SpawnObstacles(const TArray<TSubclassOf<AActor>>& ObstaclePrefabs, int32 NumToSpawn, float MovingSpeed);
    void SpawnObstacleAtRandomLocation(TSubclassOf<AActor> ObstaclePrefab, float MovingSpeed);
    bool FindRandomSpawnLocation(TSubclassOf<AActor> ObstaclePrefab, FVector& OutSpawnLocation);
};