// Fill out your copyright notice in the Description page of Project Settings.


#include "Agent.h"
#include "Observation.h"
#include "Reward.h"
#include "../Utils/PythonSocketComunicator.h"

UAgent::UAgent()
{

}

void UAgent::Init()
{
	for (const TSubclassOf<UObservation>& ObservationClass : ObservationsClass)
	{
		UObservation* NewObs = NewObject<UObservation>(this, *ObservationClass);
		Observations.Add(NewObs);

		NewObs->Init();

		TotalObsCount += NewObs->ComputeObservation().Num();
	}

	for (const TSubclassOf<UReward>& RewardClass : RewardsClass)
	{
		UReward* NewRew = NewObject<UReward>(this, *RewardClass);
		Rewards.Add(NewRew);

		NewRew->Init();
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

FState UAgent::Step()
{
	const TArray<float> ObservationsVector = UpdateObservations();
	const TArray<float> ActionsVector = UpdateActions();

	const float Reward = UpdateRewards();
	
	return FState(ObservationsVector, Reward, ActionsVector);
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

	return TotalReward;
}

TArray<float> UAgent::UpdateActions()
{
	// TODO
	TArray<float> Actions;
	Actions.Init(0.f, 2);

	return Actions;
}
