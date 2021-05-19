// Fill out your copyright notice in the Description page of Project Settings.

#include "PythonSocketComunicator.h"

#include "cstring"
#include <ws2tcpip.h>
#include <string>

#define LOCAL_IP "127.0.0.1"
#define DEFAULT_PORT "8080"

#define SOCKET_BUFFER_SIZE 512

#pragma optimize("", off)


PythonSocketComunicator::PythonSocketComunicator()
	: RecvBuffer(SOCKET_BUFFER_SIZE, 0)
{
	if (WSAStartupCounter == 0)
	{
		const int32 StartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (StartupResult != 0)
		{
			check(0 && "Failed to startup WinSocket");
		}

		WSAStartupCounter++;
	}
}

PythonSocketComunicator::~PythonSocketComunicator()
{
	WSAStartupCounter--;

	if (IsConnected())
	{
		Disconnect();
	}

	if (WSAStartupCounter == 0)
	{
		WSACleanup();
	}
}

bool PythonSocketComunicator::IsConnected() const
{
	return ConnectionSocket != INVALID_SOCKET;
}

bool PythonSocketComunicator::Connect()
{
	addrinfo* result = nullptr;
	addrinfo* ptr = nullptr;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	const int AddrResult = getaddrinfo(LOCAL_IP, DEFAULT_PORT, &hints, &result);
	if (AddrResult != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Socket failed to connect."));
		WSACleanup();
		return false;
	}

	ptr = result;

	ConnectionSocket = socket(AF_INET, SOCK_STREAM, 0); //socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectionSocket == INVALID_SOCKET)
	{
		UE_LOG(LogTemp, Error, TEXT("Error at socket: %ld\n"), WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	freeaddrinfo(result);

	const int ConnectionResult = connect(ConnectionSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (ConnectionResult == SOCKET_ERROR) 
	{
		UE_LOG(LogTemp, Error, TEXT("Error opening socket connection."));
		closesocket(ConnectionSocket);
		ConnectionSocket = INVALID_SOCKET;
	}

	return IsConnected();
}

bool PythonSocketComunicator::Disconnect()
{
	if (IsConnected())
	{
		const int SocketClosed = closesocket(ConnectionSocket);
		if (SocketClosed != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Error closing socket: %d\n"), SocketClosed);
			return false;
		}
	}

	return true;
}

bool PythonSocketComunicator::SendData(const char* Data, const int32 Size)
{
	if (IsConnected())
	{
		const int DataSent = send(ConnectionSocket, Data, Size, 0);

		if (DataSent == SOCKET_ERROR)
		{
			UE_LOG(LogTemp, Error, TEXT("send failed : %d"), WSAGetLastError());
			closesocket(ConnectionSocket);
			WSACleanup();
			return false;
		}

		UE_LOG(LogTemp, Log, TEXT("Bytes sent: %d"), DataSent);
		return true;
	}

	return false;
}

bool PythonSocketComunicator::SendQuit()
{
	UE_LOG(LogTemp, Warning, TEXT("PythonSocketComunicator::SendQuit(): Quitting Connection."));
	return SendData(-1);
}

void PythonSocketComunicator::SetMode(PythonSocketComunicator::Mode NewMode)
{
	if (IsConnected())
	{
		char* NewTypeStr = "";

		if (NewMode == PythonSocketComunicator::Mode::Inference)
		{
			UE_LOG(LogTemp, Log, TEXT("Requesting Mode Inference"));
			NewTypeStr = "inference";
		}
		else if (NewMode == PythonSocketComunicator::Mode::Training)
		{
			UE_LOG(LogTemp, Log, TEXT("Requesting Mode Training"));
			NewTypeStr = "training";
		}
		else if (NewMode == PythonSocketComunicator::Mode::Backward)
		{
			UE_LOG(LogTemp, Log, TEXT("Requesting Mode Backward"));

			NewTypeStr = "backward";
		}

		SendData(NewTypeStr, strlen(NewTypeStr));

		// wait for acknowledge
		const int32 DataReadNum = ReadData();
		std::string Response(RecvBuffer.data(), DataReadNum);

		check(Response == "ok");
	}
}

int PythonSocketComunicator::ReadData()
{
	char* buffptr;
	return ReadData(SOCKET_BUFFER_SIZE, buffptr);
}

int PythonSocketComunicator::ReadData(int32 BuffSize, char*& OutBuffer)
{
	const int ReceivedByte = recv(ConnectionSocket, RecvBuffer.data(), BuffSize, 0);

	if (ReceivedByte > 0)
	{
		UE_LOG(LogTemp, Verbose, TEXT("Bytes Received: %d"), ReceivedByte);
		OutBuffer = RecvBuffer.data();
		return ReceivedByte;
	}
	else if (ReceivedByte == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Connection Close, read 0 byte"));
		return 0;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("recv failed %ld"), WSAGetLastError());
		return -1;
	}
}

const std::vector<char>& PythonSocketComunicator::GetBuffer() const
{
	return RecvBuffer;
}

int32 PythonSocketComunicator::WSAStartupCounter{0};

#undef BUFF_LEN
