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

	bool IsConnected() const;

	bool Connect();
	bool Disconnect();

	bool SendData(const char* Data, const int32 Size);
	int ReadData();

	const std::vector<char>& GetBuffer() const;

private:
	WSADATA wsaData;
	SOCKET ConnectionSocket = INVALID_SOCKET;
	
	std::vector<char> RecvBuffer;

	static int32 WSAStartupCounter;
};
