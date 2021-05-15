// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayComponent.h"
#include "EngineUtils.h"
#include "../Spawner/BallSpawner.h"

// Sets default values for this component's properties
UPlayComponent::UPlayComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlayComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayComponent::TurnOn()
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
