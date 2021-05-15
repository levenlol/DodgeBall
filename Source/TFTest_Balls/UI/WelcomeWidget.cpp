// Fill out your copyright notice in the Description page of Project Settings.


#include "WelcomeWidget.h"
#include "Components/Button.h"
#include "../Core/BallPlayerController.h"


void UWelcomeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TrainingButton->OnClicked.AddDynamic(this, &UWelcomeWidget::OnTrainingClicked);
	PlayButton->OnClicked.AddDynamic(this, &UWelcomeWidget::OnPlayClicked);

}

void UWelcomeWidget::NativeDestruct()
{
	Super::NativeDestruct();

	TrainingButton->OnClicked.RemoveDynamic(this, &UWelcomeWidget::OnTrainingClicked);
	PlayButton->OnClicked.RemoveDynamic(this, &UWelcomeWidget::OnPlayClicked);
}

void UWelcomeWidget::OnTrainingClicked()
{
	if (ABallPlayerController* PlayerController = GetOwningPlayer<ABallPlayerController>())
	{
		PlayerController->NotifyStartTraining();
	}
}

void UWelcomeWidget::OnPlayClicked()
{
	if (ABallPlayerController* PlayerController = GetOwningPlayer<ABallPlayerController>())
	{
		PlayerController->NotifyStartPlay();
	}
}
