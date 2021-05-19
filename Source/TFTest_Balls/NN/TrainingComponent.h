// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NNCommon.h"
#include "TrainingComponent.generated.h"

class UAgent;
class PythonSocketComunicator;
struct FState;
class ABaseBallPawn;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TFTEST_BALLS_API UTrainingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrainingComponent();

	void TurnOn();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = NN)
	TSubclassOf<UAgent> AgentClass;

	// Frames
	UPROPERTY(EditDefaultsOnly, Category = NN)
	int32 ExperienceLength = 3600;

	UPROPERTY(EditDefaultsOnly, Category = NN)
	float LearningRate = 0.001f;

private:
	void SpawnAgentToTrain();

	void InitTrainingComponent();
	void InitSocketConnection();

	void PossesPawn(ABaseBallPawn* Pawn);

	void InitBallSpawners();

	void StartExperience();
	void EndExperience();

	UPROPERTY()
	UAgent* AgentToTrain = nullptr;

	UPROPERTY(VisibleAnywhere, Category = NN)
	TArray<FState> States;

	TSharedPtr<PythonSocketComunicator> Socket;
	int32 CurrentExperienceFrame = 0;
};
