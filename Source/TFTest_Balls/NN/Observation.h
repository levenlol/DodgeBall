// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Observation.generated.h"

class ABaseBallPawn;


UCLASS(Blueprintable, BlueprintType)
class TFTEST_BALLS_API UObservation : public UObject
{
	GENERATED_BODY()
public:
	UObservation();

	virtual void Init(ABaseBallPawn* InPawn);

	virtual TArray<float> ComputeObservation() const;
protected:
	UPROPERTY()
	ABaseBallPawn* Pawn = nullptr;
};
