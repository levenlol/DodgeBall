// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingComponent.h"
#include "../Spawner/BallSpawner.h"
#include "EngineUtils.h"
#include "Agent.h"
#include "../Utils/PythonSocketComunicator.h"

// Sets default values for this component's properties
UTrainingComponent::UTrainingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called when the game starts
void UTrainingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTrainingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentExperienceFrame == 0)
	{
		StartExperience();
	}

	// update observation / rewards
	FState State = AgentToTrain->Step();
	States.Add(State);
	CurrentExperienceFrame++;

	if (CurrentExperienceFrame == ExperienceLength)
	{
		EndExperience();
		CurrentExperienceFrame = 0;
	}
}

void UTrainingComponent::TurnOn()
{
	UE_LOG(LogTemp, Log, TEXT("TRAINING STARTED."));

	Activate();

	// activate ball spawners
	InitBallSpawners();

	SpawnAgentToTrain();

	Socket = MakeShared<PythonSocketComunicator>();
}

void UTrainingComponent::SpawnAgentToTrain()
{
	AgentToTrain = NewObject<UAgent>(this, *AgentClass);
	AgentToTrain->Init();
}

void UTrainingComponent::InitBallSpawners()
{
	for (TActorIterator<ABallSpawner> It(GetWorld(), ABallSpawner::StaticClass()); It; ++It)
	{
		ABallSpawner* Spawner = *It;
		if (Spawner)
		{
			Spawner->Start();
		}
	}
}

void UTrainingComponent::StartExperience()
{
	AgentToTrain->StartExperience(Socket);
}

void UTrainingComponent::EndExperience()
{
	AgentToTrain->EndExperience();
}
