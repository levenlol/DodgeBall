// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Observation.generated.h"


UCLASS(Blueprintable, BlueprintType)
class TFTEST_BALLS_API UObservation : public UObject
{
	GENERATED_BODY()
public:
	UObservation();

	virtual void Init();

	virtual TArray<float> ComputeObservation() const;
};
