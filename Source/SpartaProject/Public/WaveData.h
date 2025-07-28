#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveData.generated.h"

USTRUCT(BlueprintType)
struct FWaveData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<AActor>> StaticObstaclePrefabs;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NumStaticObstaclesToSpawn;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSubclassOf<AActor>> MovingObstaclePrefabs;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NumMovingObstaclesToSpawn;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovingObstacleSpeed;
};