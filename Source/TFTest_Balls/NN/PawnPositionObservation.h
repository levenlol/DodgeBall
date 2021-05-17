// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Observation.h"
#include "PawnPositionObservation.generated.h"

class ABaseBallPawn;


UCLASS()
class TFTEST_BALLS_API UPawnPositionObservation : public UObservation
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Setup)
	FVector2D LongitudinalLimits;

	UPROPERTY(EditAnywhere, Category = Setup)
	FVector2D LateralLimits;

	virtual void Init() override;

	TArray<float> ComputeObservation() const override;
private:
	UPROPERTY()
	ABaseBallPawn* BallPawn = nullptr;
};
