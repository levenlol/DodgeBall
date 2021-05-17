// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NNCommon.h"
#include "Agent.generated.h" 

class UObservation;
class UReward;
class PythonSocketComunicator;

UCLASS(Blueprintable, BlueprintType)
class TFTEST_BALLS_API UAgent : public UObject
{
	GENERATED_BODY()
public:
	UAgent();

	UPROPERTY(EditAnywhere, Category = NN)
	TArray<TSubclassOf<UObservation>> ObservationsClass;

	UPROPERTY(EditAnywhere, Category = NN)
	TArray<TSubclassOf<UReward>> RewardsClass;

	// Up - Down
	UPROPERTY(EditAnywhere, Category = NN)
	int32 ActionsSize = 2;

	void Init();

	void StartExperience(TSharedPtr<PythonSocketComunicator> InSocket);
	void EndExperience();

	FState Step();

private:

	UPROPERTY(VisibleAnywhere, Category = NN)
	TArray<UObservation*> Observations;

	UPROPERTY(VisibleAnywhere, Category = NN)
	TArray<UReward*> Rewards;

	UPROPERTY(VisibleAnywhere, Category = NN)
	int32 TotalObsCount = 0;

	TArray<float> UpdateObservations();
	float UpdateRewards();
	TArray<float> UpdateActions();

	TSharedPtr<PythonSocketComunicator> Socket;
};

