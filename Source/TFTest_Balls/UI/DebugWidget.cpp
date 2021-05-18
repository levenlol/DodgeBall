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

		// build NN
		SocketComunicator.SendData(7200);		// input space
		SocketComunicator.SendData(5);			// action space
		SocketComunicator.SendData(0.001f);		// learning rate
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

	TArray<float> A;
	A.Init(0.f, 7200);
	
	bool bSendResult = true;
	for (float a : A)
	{
		bSendResult &= SocketComunicator.SendData(a);
	}

	UE_LOG(LogTemp, Log, TEXT("Reading data"));
	ReadData();

	//bSendResult &= SocketComunicator.SendQuit();
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
		int32 ActionValueIdx;
		const int32 FirstActionNum = SocketComunicator.ReadData(ActionValueIdx);

		UE_LOG(LogTemp, Log, TEXT("Action Number %d is: %d"), i, ActionValueIdx);
	}
}
