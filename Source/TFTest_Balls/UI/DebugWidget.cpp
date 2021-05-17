// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWidget.h"
#include "Components/Button.h"
#include <string>

void UDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConnectButton->OnClicked.AddDynamic(this, &UDebugWidget::OnConnectClicked);
	SendDataButton->OnClicked.AddDynamic(this, &UDebugWidget::SendData);
	ReadDataButton->OnClicked.AddDynamic(this, &UDebugWidget::ReadData);

	OnConnectClicked();
}

void UDebugWidget::NativeDestruct()
{
	Super::NativeDestruct();

	ConnectButton->OnClicked.RemoveDynamic(this, &UDebugWidget::OnConnectClicked);
	SendDataButton->OnClicked.RemoveDynamic(this, &UDebugWidget::SendData);
	ReadDataButton->OnClicked.RemoveDynamic(this, &UDebugWidget::ReadData);
}

void UDebugWidget::OnConnectClicked()
{
	const bool bResult = SocketComunicator.Connect();
	if (bResult)
	{
		UE_LOG(LogTemp, Log, TEXT("Socket Connected."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket failed to Connect."));
	}
}

void UDebugWidget::SendData()
{
	// set mode to inference
	UE_LOG(LogTemp, Log, TEXT("Set mode to inference"));

	SocketComunicator.SetMode(PythonSocketComunicator::Inference);

	UE_LOG(LogTemp, Log, TEXT("Sending observations."));

	TArray<float> A({ 4.0f, 23.f, 12.f, 11.f });
	
	bool bSendResult = SocketComunicator.SendData(A.Num());
	for (float a : A)
	{
		bSendResult &= SocketComunicator.SendData(a);
	}

	UE_LOG(LogTemp, Log, TEXT("Reading data"));
	ReadData();

	bSendResult &= SocketComunicator.SendQuit();
}

void UDebugWidget::ReadData()
{
	// first read num of actions (2)
	char* BufferPtr = nullptr;
	const int32 ReadDataNum = SocketComunicator.ReadData(4, BufferPtr);

	int32 ActionsNum = -1;
	memcpy(&ActionsNum, BufferPtr, sizeof(int32));

	for (int32 i = 0; i < ActionsNum; i++)
	{
		float ActionValue;
		const int32 FirstActionNum = SocketComunicator.ReadData(ActionValue);

		UE_LOG(LogTemp, Log, TEXT("Action Number %d is: %f"), i, ActionValue);
	}
}
