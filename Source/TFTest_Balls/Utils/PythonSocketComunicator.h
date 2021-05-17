// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "stdio.h"
#include <WinSock2.h>
#include <vector>


class TFTEST_BALLS_API PythonSocketComunicator
{
public:
	PythonSocketComunicator();
	~PythonSocketComunicator();

	enum Mode
	{
		Inference = 0,
		Backward = 1
	};

	bool IsConnected() const;

	bool Connect();
	bool Disconnect();

	bool SendData(const char* Data, const int32 Size);
	
	template<typename T>
	bool SendData(T data);

	bool SendQuit();

	void SetMode(PythonSocketComunicator::Mode NewMode);

	int ReadData();

	int ReadData(int32 BuffSize, char*& OutBuffer);

	template<typename T>
	int ReadData(T& OutValue);

	const std::vector<char>& GetBuffer() const;

private:
	WSADATA wsaData;
	SOCKET ConnectionSocket = INVALID_SOCKET;
	
	std::vector<char> RecvBuffer;

	static int32 WSAStartupCounter;
};

template<typename T>
bool PythonSocketComunicator::SendData(T Data)
{
	return SendData((char*) &Data, sizeof(Data));
}

template<typename T>
int PythonSocketComunicator::ReadData(T& OutValue)
{
	char* BuffPtr = nullptr;
	const int32 ReadDataNum = ReadData(sizeof(T), BuffPtr);

	if (ReadDataNum != sizeof(T))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Expected %d bytes, got %d."), sizeof(T), ReadDataNum);
	}

	memcpy(&OutValue, BuffPtr, sizeof(T));

	return ReadDataNum;
}
