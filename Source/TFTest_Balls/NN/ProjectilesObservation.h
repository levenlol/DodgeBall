// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Observation.h"
#include "ProjectilesObservation.generated.h"

class ABallSpawner;
class ABaseBallPawn;

/**
 * 
 */
UCLASS()
class TFTEST_BALLS_API UProjectilesObservation : public UObservation
{
	GENERATED_BODY()
public:

	virtual void Init(ABaseBallPawn* InPawn) override;

	TArray<float> ComputeObservation() const override;

	UPROPERTY(EditAnywhere, Category = Setup)
	FVector2D LongitudinalLimits;

	UPROPERTY(EditAnywhere, Category = Setup)
	FVector2D LateralLimits;

	// X - Y
	UPROPERTY(EditAnywhere, Category = Setup)
	FVector2D SpeedLimits;

private:
	UPROPERTY()
	TArray<ABallSpawner*> BallSpawners;
};
