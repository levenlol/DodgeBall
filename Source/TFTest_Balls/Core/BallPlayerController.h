// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BallPlayerController.generated.h"

class UWelcomeWidget;

/**
 * 
 */
UCLASS()
class TFTEST_BALLS_API ABallPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Start)
	TSubclassOf<UWelcomeWidget> WelcomeWidgetClass;

	void BeginPlay();

	void NotifyStartTraining();
	void NotifyStartPlay();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_NotifyStartTraining();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_NotifyStartPlay();
	
	UPROPERTY()
	UWelcomeWidget* WelcomeWidget = nullptr;
};
