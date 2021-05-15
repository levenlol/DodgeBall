// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPlayerController.h"
#include "../UI/WelcomeWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "../NN/TrainingComponent.h"
#include "../Core/PlayComponent.h"
#include "RHI.h"

void ABallPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GUsingNullRHI)
	{
		NotifyStartTraining();
	}
	else
	{
		check(WelcomeWidgetClass);
		if (WelcomeWidgetClass)
		{
			WelcomeWidget = CreateWidget<UWelcomeWidget>(this, WelcomeWidgetClass);
			WelcomeWidget->AddToViewport();
		}

		bShowMouseCursor = true;
	}
}

void ABallPlayerController::NotifyStartTraining()
{
	Server_NotifyStartTraining();
	bShowMouseCursor = false;

	if (!GUsingNullRHI)
	{
		WelcomeWidget->RemoveFromViewport();
	}
}

void ABallPlayerController::NotifyStartPlay()
{
	Server_NotifyStartPlay();
	bShowMouseCursor = false;
	WelcomeWidget->RemoveFromViewport();
}

void ABallPlayerController::Server_NotifyStartTraining_Implementation()
{
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		if (UTrainingComponent* TrainingComponent = GameMode->FindComponentByClass<UTrainingComponent>())
		{
			TrainingComponent->TurnOn();
		}
	}
}

void ABallPlayerController::Server_NotifyStartPlay_Implementation()
{
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		if (UPlayComponent* PlayComponent = GameMode->FindComponentByClass<UPlayComponent>())
		{
			PlayComponent->TurnOn();
		}
	}
}

bool ABallPlayerController::Server_NotifyStartTraining_Validate() { return true; }
bool ABallPlayerController::Server_NotifyStartPlay_Validate() { return true; }