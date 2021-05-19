// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilesObservation.h"
#include "../Spawner/BallSpawner.h"
#include "EngineUtils.h"
#include "../Spawner/BallProjectile.h"
#include "../Core/BaseBallPawn.h"

void UProjectilesObservation::Init(ABaseBallPawn* InPawn)
{
	Super::Init(InPawn);

	for (TActorIterator<ABallSpawner> It(GetWorld(), ABallSpawner::StaticClass()); It; ++It)
	{
		ABallSpawner* Spawner = *It;
		BallSpawners.Add(Spawner);
	}
}

TArray<float> UProjectilesObservation::ComputeObservation() const
{
	TArray<float> ObsView;

	for (ABallSpawner* Spawner : BallSpawners)
	{
		const TArray<ABallProjectile*>& Projectiles = Spawner->GetBallProjectiles();

		for (ABallProjectile* Projectile : Projectiles)
		{
			const FVector Position = Projectile->GetActorLocation();
			const float X = FMath::GetMappedRangeValueUnclamped(LongitudinalLimits, FVector2D(-1.f, 1.f), Position.X);
			const float Y = FMath::GetMappedRangeValueUnclamped(LateralLimits, FVector2D(-1.f, 1.f), Position.Y);

			ObsView.Add(X);
			ObsView.Add(Y);

			const FVector Speed = Projectile->GetProjectileSpeed();
			const float VelX = FMath::GetMappedRangeValueUnclamped(SpeedLimits, FVector2D(-1.f, 1.f), Speed.X);
			const float VelY = FMath::GetMappedRangeValueUnclamped(SpeedLimits, FVector2D(-1.f, 1.f), Speed.Y);

			ObsView.Add(VelX);
			ObsView.Add(VelY);
		}
	}

	return ObsView;
}
