// Fill out your copyright notice in the Description page of Project Settings.

#include "PythonSocketComunicator.h"

#include "cstring"
#include <ws2tcpip.h>

#define LOCAL_IP "127.0.0.1"
#define DEFAULT_PORT "8080"

#define SOCKET_BUFFER_SIZE 512

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
			UE_LOG(LogTemp, Error, TEXT("send failed : %d \n"), WSAGetLastError());
			closesocket(ConnectionSocket);
			WSACleanup();
			return false;
		}

		UE_LOG(LogTemp, Log, TEXT("Bytes sent: %d\n"), DataSent);
		return true;
	}

	return false;
}

int PythonSocketComunicator::ReadData()
{
	const int ReceivedByte = recv(ConnectionSocket, RecvBuffer.data(), SOCKET_BUFFER_SIZE, 0);
	if (ReceivedByte > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Bytes Received: %d - Bytes: %s"), *FString(ReceivedByte, RecvBuffer.data()));
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
