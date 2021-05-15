// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Utils/PythonSocketComunicator.h"
#include "DebugWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class TFTEST_BALLS_API UDebugWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UButton* ConnectButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* SendDataButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* ReadDataButton = nullptr;

	void NativeConstruct() override;
	void NativeDestruct() override;

private:
	UFUNCTION()
	void OnConnectClicked();

	UFUNCTION()
	void SendData();

	UFUNCTION()
	void ReadData();

	PythonSocketComunicator SocketComunicator;
};
