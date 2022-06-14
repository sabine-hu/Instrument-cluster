/**
 *
 *		@file		SocketCommunication_Cpp.cpp
 *  	@author 	Pravaig Dynamics HMI Software Development Team
 *  	@date 		09-Apr-2022
 *  	@brief		Socket communication to the backend-server
 *	@Team			Manoj Kumar Pradhan, Sabine hu, Shamik, Kartikay				
 *
 */

 /******************************************************************************
 * 	 ______     ______             _      _             _______      ____
 * 	|_____ \   |  ___ \      \    | |    | |    \      |__   __|    / ___\
 * 	 _____| |  | | __| |    / \    \ \  / /    / \        | |      / / ___
 * 	|  ____/   | || __/    / \ \    \ \/ /    / \ \       | |     | | |__ |
 * 	| |        | | \ \    / / \ \    \  /    / / \ \    __| |__   | |___| |
 * 	|_|        |_|  \_\  /_/   \_\    \/    /_/   \_\  |_______|   \_____/
 *
 *THE CODE WRITTEN IN THIS FILE IS AN INTELLECTUAL PROPERTY OF PRAVAIG DYNAMICS.
 *THE CODE OR THE FILE SHOULD NOT BE SHARED WITH ANY PERSON NOT AFFILIATED TO
 *PRAVAIG DYNAMICS. THE CODE OR FILE SHOULD NOT BE UPLOADED ON OR COPIED TO
 *ANY PUBLIC CODE REPOSITORY, PERSONAL CODE REPOSITORY, ANY EXTERNAL MEMORY
 *STORAGE DEVICE OR PERSONAL CLOUD STORAGE.
 ******************************************************************************/


#include "SocketCommunication_Cpp.h"

#include "HAL/RunnableThread.h"
#include "Async/Async.h"
#include "HAL/UnrealMemory.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include <string>
#include "Templates/Tuple.h"



// Sets default values for this component's properties
ASocketCommunication_Cpp::ASocketCommunication_Cpp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = false;

}

void ASocketCommunication_Cpp::BeginPlay()
{
	Super::BeginPlay();
}

void ASocketCommunication_Cpp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<int32> keys;
	TcpWorkers.GetKeys(keys);

	for (auto& key : keys)
	{
		Disconnect(key);
	}
}

void ASocketCommunication_Cpp::Connect(const FString& ipAddress, int32 port, int32& ConnectionId)
{
	TWeakObjectPtr<ASocketCommunication_Cpp> thisWeakObjPtr = TWeakObjectPtr<ASocketCommunication_Cpp>(this);
	TSharedRef<FTcpSocket> worker(new FTcpSocket(ipAddress, port, thisWeakObjPtr, ConnectionId, ReceiveBufferSize, SendBufferSize, TimeBetweenTicks));
	TcpWorkers.Add(ConnectionId, worker);
	worker->Start();

}

void ASocketCommunication_Cpp::Disconnect(int32 ConnectionId)
{
	auto worker = TcpWorkers.Find(ConnectionId);
	if (worker)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Tcp Socket :  Disconnected from server"));
		worker->Get().Stop();
		TcpWorkers.Remove(ConnectionId);
	}
}

bool ASocketCommunication_Cpp::SendData(int32 ConnectionId, FPacketData DataToSend)
{
	TcpWorkers[ConnectionId]->AddToOutbox(DataToSend);
	if (TcpWorkers.Contains(ConnectionId))
	{
		if (TcpWorkers[ConnectionId]->isConnected())
		{
			//TcpWorkers[ConnectionId]->AddToOutbox(DataToSend);
			return true;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Socket is not connected!"));
			return false;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Socket doesn't Exist"));
		return false;
	}
}

TArray<FPacketData> ASocketCommunication_Cpp::ReceiveData(int32 ConnectionId)
{
	return TcpWorkers[ConnectionId].Get().ReadFromInbox();
}

bool ASocketCommunication_Cpp::IsConnected(int32 ConnectionId)
{
	return TcpWorkers[ConnectionId].Get().isConnected();
}



void FTcpSocket::SocketShutdown()
{
	//UE_LOG(LogTemp, Warning, TEXT("Socket SHutdown!!!"));
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

bool FTcpSocket::isConnected()
{
	return bConnected;
}

void FTcpSocket::AddToOutbox(FPacketData Message)
{
	Outbox.Enqueue(Message);
}

TArray<FPacketData> FTcpSocket::ReadFromInbox()
{
	TArray<FPacketData> msg;
	Inbox.Dequeue(msg);
	return msg;
}

FTcpSocket::FTcpSocket()
{
}

FTcpSocket::FTcpSocket(FString inIp, const int32 inPort, TWeakObjectPtr<ASocketCommunication_Cpp> InOwner, int32 inId, int32 inRecvBufferSize, int32 inSendBufferSize, float inTimeBetweenTicks)
{
	ipAddress = inIp;
	port = inPort;
	SocketOwner = InOwner;
	id = inId;
	RecvBufferSize = inRecvBufferSize;
	SendBufferSize = inSendBufferSize;
	TimeBetweenTicks = inTimeBetweenTicks;
}
	



FTcpSocket::~FTcpSocket()
{
	Stop();
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}
}

void FTcpSocket::Start()
{
	if (Thread)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Thread isn't null : %s"), *Thread->GetThreadName());
	}

	Thread = FRunnableThread::Create(this, *FString::Printf(TEXT("Started Thread TcpSocket %s:%d"), *ipAddress, port), 1024 * 1024, TPri_TimeCritical);
	//UE_LOG(LogTemp, Warning, TEXT("Thread created!"));
}

bool FTcpSocket::Init()
{
	bRun = true;
	bConnected = false;
	return true;
}

uint32 FTcpSocket::Run()
{
	//AsyncTask(ENamedThreads::GameThread, []() { UE_LOG(LogTemp, Warning, TEXT("Starting TCP socket Run() Thread"))});

	while (bRun)
	{

		if (!Outbox.IsEmpty())
		{
			FDateTime timeBeginningOfTick = FDateTime::UtcNow();

			// Connect ot the server
			if (!bConnected)
			{
				Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("UE9_Socket"), false);
				if (!Socket)
				{
					return 0;
				}

				Socket->SetReceiveBufferSize(RecvBufferSize, RecvBufferSize);
				Socket->SetSendBufferSize(SendBufferSize, SendBufferSize);

				FIPv4Address ip;
				FIPv4Address::Parse(ipAddress, ip);

				TSharedRef<FInternetAddr> internetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
				internetAddr->SetIp(ip.Value);
				internetAddr->SetPort(port);

				bConnected = Socket->Connect(*internetAddr);

				if (bConnected)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Connected to Server!!!"));
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("Unable to connect to server!"));
					bRun = false;
				}

				//continue;
			}

			if (!Socket)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Socket is null!"));
				bRun = false;
				continue;
			}

			//UE_LOG(LogTemp, Warning, TEXT("Outbox : %d"), Outbox.IsEmpty());
			// if outbox have something, send it
			while (!Outbox.IsEmpty())
			{
				FPacketData PacketToSend;
				Outbox.Dequeue(PacketToSend);

				if (bConnected)
				{
					int32 BytesSent = 0;

					PacketSent = Socket->Send((uint8*)&PacketToSend, sizeof(PacketToSend), BytesSent);

					//UE_LOG(LogTemp, Warning, TEXT("Packet sent to server : %d & Bytes sent : %d !!!"), PacketSent, BytesSent);
				}
			}

			// if we can read something from server
			uint32 PendingDataSize = 0;
			TArray<FPacketData> receivedData;
			
			// Set socket blocking
			Socket->SetNonBlocking(true);

			int32 BytesReadInTotal = 0;
			while (bRun)
			{
				if (Socket->HasPendingData(PendingDataSize) || PacketSent)
				{
					//UE_LOG(LogTemp, Warning, TEXT("Listening from Server!"));
					FPacketData PacketData;

					Socket->Recv((uint8*)&PacketData, sizeof(PacketData), BytesReadInTotal);

					if (PacketData.Command == E_VCommand::EV_Exit)
					{
						PacketSent = false;
						break;
					}
					else
					{
						receivedData.Add(PacketData);
					}
				}
				else
					break;

			}

			// Set back non blocking to false
			Socket->SetNonBlocking(false);

			// if we received data, inform the main thread about it
			if (bRun && receivedData.Num() != 0)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Received Data from Server"));
				Inbox.Enqueue(receivedData);
			}

			// In order to sleep, will account for how much this tick took due to sending and receiving
			FDateTime timeEndOfTick = FDateTime::UtcNow();
			FTimespan tickDuration = timeEndOfTick - timeBeginningOfTick;
			float secondsThisTickTook = tickDuration.GetTotalSeconds();
			float timeToSleep = TimeBetweenTicks - secondsThisTickTook;
			if (timeToSleep > 0.f)
			{
				FPlatformProcess::Sleep(timeToSleep);
			}

			bConnected = false;

			SocketShutdown();
			if (Socket)
			{
				delete Socket;
				Socket = nullptr;
			}
		}
		

	}



	return 0;

}

void FTcpSocket::Stop()
{
	bRun = false;
	bConnected = false;

	SocketShutdown();
	if (Socket)
	{
		delete Socket;
		Socket = nullptr;
	}
}

void FTcpSocket::Exit()
{
}
