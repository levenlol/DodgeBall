// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"
#include "Observation.h"
#include "Reward.h"
#include "../Utils/PythonSocketComunicator.h"
#include "../Core/BaseBallPawn.h"

UAgent::UAgent()
{

}

void UAgent::Init(ABaseBallPawn* InPawn)
{
	Pawn = InPawn;

	for (const TSubclassOf<UObservation>& ObservationClass : ObservationsClass)
	{
		UObservation* NewObs = NewObject<UObservation>(this, *ObservationClass);
		Observations.Add(NewObs);

		NewObs->Init(Pawn);

		TotalObsCount += NewObs->ComputeObservation().Num();
	}

	for (const TSubclassOf<UReward>& RewardClass : RewardsClass)
	{
		UReward* NewRew = NewObject<UReward>(this, *RewardClass);
		Rewards.Add(NewRew);

		NewRew->Init(Pawn);
	}
}

void UAgent::StartExperience(TSharedPtr<PythonSocketComunicator> InSocket)
{
	NN_Printf(TEXT("Start Experience! %d"));
	Socket = InSocket;


}

void UAgent::EndExperience()
{

}

FState UAgent::Step_Training()
{
	// set mode to training
	UE_LOG(LogTemp, Log, TEXT("Set mode to Training"));

	Socket->SetMode(PythonSocketComunicator::Training);

	const TArray<float> ObservationsVector = UpdateObservations();
	const TArray<float> ActionsVector = UpdateActions(ObservationsVector);

	const float Reward = UpdateRewards();

	UpdatePawnActions(ActionsVector);
	
	// this return is useless. just used for debug purpose.
	return FState(ObservationsVector, Reward, ActionsVector);
}

int32 UAgent::GetTotalObservationCount() const
{
	return TotalObsCount;
}

TArray<float> UAgent::UpdateObservations()
{
	TArray<float> ObservationsVector;
	ObservationsVector.Reserve(TotalObsCount);

	for (UObservation* Obs : Observations)
	{
		const TArray<float> CurrObs = Obs->ComputeObservation();

		for (int32 i = 0; i < CurrObs.Num(); i++)
		{
			ObservationsVector.Add(CurrObs[i]);
		}
	}

	check(ObservationsVector.Num() == TotalObsCount);

	return ObservationsVector;
}

float UAgent::UpdateRewards()
{
	float TotalReward = 0.f;
	for (UReward* Rew : Rewards)
	{
		TotalReward += Rew->ComputeReward();
	}

	Socket->SendData(TotalReward);

	return TotalReward;
}

TArray<float> UAgent::UpdateActions(const TArray<float>& InObservations)
{
	// prepare to run inference graph
	UE_LOG(LogTemp, Log, TEXT("Sending observations."));

	Socket->SendData((const char*)InObservations.GetData(), InObservations.Num() * sizeof(float));

	// first read num of actions (2)
	char* BufferPtr = nullptr;
	const int32 ReadDataNum = Socket->ReadData(4, BufferPtr);

	int32 ActionsRead = -1;
	memcpy(&ActionsRead, BufferPtr, sizeof(int32));

	check(ActionsNum == ActionsRead);

	TArray<float> Actions;
	Actions.Init(0.f, ActionsRead);

	for (int32 i = 0; i < ActionsRead; i++)
	{
		int32 ActionValueIdx;
		const int32 FirstActionNum = Socket->ReadData(ActionValueIdx);

		Actions[i] = static_cast<float>(FMath::Lerp(-1.f, 1.f, static_cast<float>(ActionValueIdx) / static_cast<float>(ActionsSize - 1)));
		UE_LOG(LogTemp, Log, TEXT("Action Number %d is: %f"), i, Actions[i]);
	}

	return Actions;
}

void UAgent::UpdatePawnActions(const TArray<float>& InActions)
{
	Pawn->SetMovementUp(InActions[0]);
	Pawn->SetMovementRight(InActions[0]);
}
