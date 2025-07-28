#include "MapWaveManager.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"

AMapWaveManager::AMapWaveManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AMapWaveManager::StartWave(int32 WaveNumber)
{
    if (!WaveDataTable)
    {
        return;
    }

    FString ContextString;
    FName RowName = FName(*FString::FromInt(WaveNumber));
    FWaveData* WaveData = WaveDataTable->FindRow<FWaveData>(RowName, ContextString);

    if (WaveData)
    {
        StartWaveInternal(*WaveData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("WaveData Error: %d"), WaveNumber);
    }
}

void AMapWaveManager::StartWaveInternal(const FWaveData& WaveData)
{
    SpawnObstacles(WaveData.StaticObstaclePrefabs, WaveData.NumStaticObstaclesToSpawn, 0.0f);
    SpawnObstacles(WaveData.MovingObstaclePrefabs, WaveData.NumMovingObstaclesToSpawn, WaveData.MovingObstacleSpeed);
}

void AMapWaveManager::SpawnObstacles(const TArray<TSubclassOf<AActor>>& ObstaclePrefabs, int32 NumToSpawn, float MovingSpeed)
{
    if (ObstaclePrefabs.Num() == 0 || NumToSpawn <= 0)
    {
        return;
    }

    for (int32 i = 0; i < NumToSpawn; ++i)
    {
        TSubclassOf<AActor> RandomObstaclePrefab = ObstaclePrefabs[FMath::RandRange(0, ObstaclePrefabs.Num() - 1)];
        SpawnObstacleAtRandomLocation(RandomObstaclePrefab, MovingSpeed);
    }
}

void AMapWaveManager::SpawnObstacleAtRandomLocation(TSubclassOf<AActor> ObstaclePrefab, float MovingSpeed)
{
    FVector SpawnLocation;
    if (!FindRandomSpawnLocation(ObstaclePrefab, SpawnLocation))
    {
        UE_LOG(LogTemp, Warning, TEXT("Error Speed"));
        return;
    }

    FRotator SpawnRotation = FRotator::ZeroRotator;
    AActor* SpawnedObstacle = GetWorld()->SpawnActor<AActor>(ObstaclePrefab, SpawnLocation, SpawnRotation);

    if (SpawnedObstacle && MovingSpeed > 0.0f)
    {
        UProjectileMovementComponent* MovementComp = SpawnedObstacle->FindComponentByClass<UProjectileMovementComponent>();
        
        if (MovementComp)
        {
            FVector MovementDirection = FMath::VRand();
            MovementDirection.Z = 0.0f;
            MovementDirection.Normalize();
            MovementComp->Velocity = MovementDirection * MovingSpeed;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MovementComp Error"));
        }

        MovementComp->bShouldBounce = true;
        MovementComp->Bounciness = 1.0f; 
    }
}

bool AMapWaveManager::FindRandomSpawnLocation(TSubclassOf<AActor> ObstaclePrefab, FVector& OutSpawnLocation)
{
    if (!ObstaclePrefab)
    {
        return false;
    }

    FVector ObstacleExtent = FVector(100.0f, 100.0f, 100.0f);
    UObject* DefaultObject = ObstaclePrefab->GetDefaultObject();
    
    if (DefaultObject)
    {
        AActor* DefaultActor = Cast<AActor>(DefaultObject);
        if (DefaultActor)
        {
            UBoxComponent* BoxComp = DefaultActor->FindComponentByClass<UBoxComponent>();
            if (BoxComp)
            {
                ObstacleExtent = BoxComp->GetScaledBoxExtent();
            }
            else
            {
                UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(DefaultActor->GetRootComponent());
                if (PrimitiveComp)
                {
                    ObstacleExtent = PrimitiveComp->Bounds.BoxExtent;
                }
            }
        }
    }

    for (int32 i = 0; i < MaxSpawnAttempts; ++i)
    {
        FVector RandomLocation = FVector(
            FMath::RandRange(MapBoundsMin.X, MapBoundsMax.X),
            FMath::RandRange(MapBoundsMin.Y, MapBoundsMax.Y),
            SpawnZLocation
        );

        FHitResult HitResult;
        FCollisionShape BoxShape = FCollisionShape::MakeBox(ObstacleExtent);
        bool bHit = GetWorld()->SweepSingleByChannel(
            HitResult,
            RandomLocation,
            RandomLocation,
            FQuat::Identity,
            ECollisionChannel::ECC_Visibility,
            BoxShape
        );

        if (!bHit)
        {
            OutSpawnLocation = RandomLocation;
            return true;
        }
    }

    return false;
}