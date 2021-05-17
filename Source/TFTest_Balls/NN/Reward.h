// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Reward.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, BlueprintType)
class TFTEST_BALLS_API UReward : public UObject
{
	GENERATED_BODY()
public:
	UReward();

	virtual void Init();
	virtual float ComputeReward() const;

};
