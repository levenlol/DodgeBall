// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainingComponent.h"
#include "../Spawner/BallSpawner.h"
#include "EngineUtils.h"
#include "Agent.h"
#include "../Utils/PythonSocketComunicator.h"
#include "../Core/BaseBallPawn.h"

#include "AIController.h"
#include "GameFramework/PlayerController.h"

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
	FState State = AgentToTrain->Step_Training();
	States[CurrentExperienceFrame] = State;
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
	InitTrainingComponent();


	// activate ball spawners
	InitBallSpawners();
	SpawnAgentToTrain();
	InitSocketConnection();
}

void UTrainingComponent::InitTrainingComponent()
{
	GEngine->SetMaxFPS(60.f);
	States.SetNum(ExperienceLength);
}

void UTrainingComponent::InitSocketConnection()
{
	// Make socket
	Socket = MakeShared<PythonSocketComunicator>();
	Socket->Connect();

	// Init NN
	Socket->SendData(AgentToTrain->GetTotalObservationCount());			// input space
	Socket->SendData(AgentToTrain->ActionsSize);						// action space
	Socket->SendData(LearningRate);										// learning rate
}

void UTrainingComponent::SpawnAgentToTrain()
{
	for (TActorIterator<ABaseBallPawn> It(GetWorld(), ABaseBallPawn::StaticClass()); It; ++It)
	{
		ABaseBallPawn* Pawn = *It;

		PossesPawn(Pawn);

		AgentToTrain = NewObject<UAgent>(this, *AgentClass);
		AgentToTrain->Init(Pawn);

		break;
	}
}

void UTrainingComponent::PossesPawn(ABaseBallPawn* Pawn)
{
	APlayerController* CurrentController = Cast<APlayerController>(Pawn->GetController());

	AAIController* Controller = GetWorld()->SpawnActor<AAIController>();
	Controller->Possess(Pawn);

	CurrentController->bAutoManageActiveCameraTarget = false;
	CurrentController->SetViewTarget(Pawn);
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
