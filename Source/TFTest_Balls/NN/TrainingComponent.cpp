// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingComponent.h"
#include "../Spawner/BallSpawner.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
UTrainingComponent::UTrainingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTrainingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTrainingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTrainingComponent::TurnOn()
{
	UE_LOG(LogTemp, Log, TEXT("TRAINING STARTED."));

	// activate ball spawners
	for (TActorIterator<ABallSpawner> It(GetWorld(), ABallSpawner::StaticClass()); It; ++It)
	{
		ABallSpawner* Spawner = *It;
		if (Spawner)
		{
			Spawner->Start();
		}
	}
}