// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NNCommon.h"
#include "Agent.generated.h" 

class UObservation;
class UReward;
class PythonSocketComunicator;
class ABaseBallPawn;

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

	UPROPERTY(EditAnywhere, Category = NN)
	int32 ActionsNum = 2;

	// Quantized Action from -1 to +1 with Steps
	UPROPERTY(EditAnywhere, Category = NN)
	int32 ActionsSize = 5;

	void Init(ABaseBallPawn* InPawn);

	void StartExperience(TSharedPtr<PythonSocketComunicator> InSocket);
	void EndExperience();

	FState Step_Training();

	int32 GetTotalObservationCount() const;

private:

	UPROPERTY(VisibleAnywhere, Category = NN)
	TArray<UObservation*> Observations;

	UPROPERTY(VisibleAnywhere, Category = NN)
	TArray<UReward*> Rewards;

	UPROPERTY(VisibleAnywhere, Category = NN)
	int32 TotalObsCount = 0;

	UPROPERTY(VisibleAnywhere, Category = NN)
	ABaseBallPawn* Pawn = nullptr;

	TArray<float> UpdateObservations();
	float UpdateRewards();
	TArray<float> UpdateActions(const TArray<float>& Observations);

	void UpdatePawnActions(const TArray<float>& InActions);

	TSharedPtr<PythonSocketComunicator> Socket;
};

