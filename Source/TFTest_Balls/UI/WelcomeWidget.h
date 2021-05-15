// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WelcomeWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class TFTEST_BALLS_API UWelcomeWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* TrainingButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton = nullptr;

	void NativeConstruct() override;
	void NativeDestruct() override;

private:
	UFUNCTION()
	void OnTrainingClicked();

	UFUNCTION()
	void OnPlayClicked();
};
