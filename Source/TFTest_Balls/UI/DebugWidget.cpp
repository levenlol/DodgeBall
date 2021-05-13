// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugWidget.h"
#include "Components/Button.h"

void UDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConnectButton->OnClicked.AddDynamic(this, &UDebugWidget::OnConnectClicked);
	SendDataButton->OnClicked.AddDynamic(this, &UDebugWidget::SendData);
	ReadDataButton->OnClicked.AddDynamic(this, &UDebugWidget::ReadData);
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
	const char* HelloData = "SAY HELLO MY FRIEND\n";
	
	const bool bSendResult = SocketComunicator.SendData(HelloData, strlen(HelloData));
}

void UDebugWidget::ReadData()
{
	SocketComunicator.ReadData();
}
