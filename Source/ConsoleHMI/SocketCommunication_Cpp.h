/**
 *
 *		@file		SocketCommunication_Cpp.h
 *  	@author 	Pravaig Dynamics HMI Software Development Team
 *  	@date 		09-Apr-2022
 *  	@brief		Header file for Socket communication to the backend-server
 *    @Team			Manoj Kumar Pradhan, Sabine hu, Shamik, Kartikay
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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Async/Async.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "Networking.h"
#include "Containers/Queue.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "Misc/FileHelper.h"
#include "IPAddress.h"
#include "HAL/ThreadSafeBool.h"
#include "HAL/Runnable.h"

#include "SocketCommunication_Cpp.generated.h"


/*
	Enum variable to signify the vechicle status data while communicating with server
*/
UENUM()
enum class E_VStatus : uint16 {

	EV_VS_SunRoofState = 0,
	EV_VS_FrontRightDoorState  = 1,
	EV_VS_MotorTemerature = 2,
	EV_VS_WornBrakeLiningMalfunction = 3,
	EV_VS_Range = 4,
	EV_VS_BatteryVoltage = 5,
	EV_VS_MotorTorque = 6,
	EV_VS_LeftIndicator = 7,
	EV_VS_Speed = 8,
	EV_VS_FrontFogLamp = 9,
	EV_VS_RearFogLamp = 10,
	EV_VS_HighBeamHeadLamp = 11,
	EV_VS_Mode = 12,
	EV_VS_PositionLamp = 13,
	EV_VS_BatteryChargeStatus = 14,
	EV_VS_Gear = 15,
	EV_VS_BrakeMalfunction = 16,
	EV_VS_MotorRPM = 17,
	EV_VS_DippedBeamHeadLamp = 18,
	EV_VS_Error = 19,
	EV_VS_FrontLeftDoorState = 20,
	EV_VS_MotorVoltage = 21,
	EV_VS_RearLeftDoorState = 22,
	EV_VS_BatterySOC = 23,
	EV_VS_TrunkState = 24,
	EV_VS_RightIndicator = 25,
	EV_VS_RearRightDoorState = 26,
	EV_VS_Regen = 27,
	EV_VS_HoodState = 28,
	EV_VS_AbsMalfunction = 29,
	EV_VS_AmbientTemperature = 30,
	EV_VS_ParkingLamp = 31		

};


/*
	Enum variable to signify the vechicle command data while communicating with the server
*/
UENUM()
enum class E_VCommand : uint16 {

	EV_Request = 0,
	EV_Modify = 1,
	EV_Can_Send = 2,
	EV_RequestAll = 3,
	EV_Error = 4,
	EV_Data = 5,
	EV_Exit = 6,
	EV_Updated = 7

};


/*
	Packet structure data that will be used for data packet processing while communicating with the server
*/
USTRUCT()
struct FPacketData
{
	GENERATED_BODY()

	// Variable to store the vehicle command data
	E_VCommand Command;

	// Variable to store the vehicle status data
	E_VStatus Status;

	// Variable to store the data of the packet
	uint32 Data;

	// Initializing the packet data with default values
	FPacketData()
	{
		Command = E_VCommand::EV_Error;
		Status = E_VStatus::EV_VS_MotorVoltage;
		Data = 0;

	}

};



// Event on blueprint - On successful connection to the server from client
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNetEventConnected);

// Event on Blueprint - On receiving packetsdata from server
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNetMsgEventPacketsData, const TArray<FPacketData>&, PacketsData);



UCLASS(Blueprintable, BlueprintType)
class CONSOLEHMI_API ASocketCommunication_Cpp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ASocketCommunication_Cpp();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	// Variable to store client socket name
	UPROPERTY()
		FString ClientSocketName;

	// Maximum Buffer Size to send and receive data for client-server communication
	UPROPERTY()
		int32 BufferMaxSize;

	// Parameter to check if client is connected or not
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Socket Connection")
		bool bIsConnected;

	// Array to store all the data received from server, on client's request
	UPROPERTY()
		TArray<FPacketData> PacketDataToReturn;

	UPROPERTY()
		FNetMsgEventPacketsData OnReceivedPacketsData;

	// Event on connected
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable)
		FNetEventConnected OnConnected;

	/* Buffer size in bytes. Currently not used. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
		int32 SendBufferSize = 256;

	/* Buffer size in bytes. It's set only when creating a socket, never afterwards. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
		int32 ReceiveBufferSize = 256;

	/* Time between ticks. Please account for the fact that it takes 1ms to wake up on a modern PC, so 0.01f would effectively be 0.011f */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
		float TimeBetweenTicks = 0.012f;

public:

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void Connect(const FString& ipAddress, int32 port, int32& ConnectionId);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void Disconnect(int32 ConnectionId);

	bool SendData(int32 ConnectionId, FPacketData DataToSend);

	TArray<FPacketData> ReceiveData(int32 ConnectionId);

	bool IsConnected(int32 ConnectionId);


private:

	TMap<int32, TSharedRef<class FTcpSocket, ESPMode::Fast>> TcpWorkers{ {0, MakeShared<FTcpSocket>() } };

};


class FTcpSocket : public FRunnable, public TSharedFromThis<FTcpSocket>
{
	FRunnableThread* Thread = nullptr;

private:

	class FSocket* Socket = nullptr;

	FString ipAddress;
	int port;

	int32 id;
	int32 RecvBufferSize;
	int32 SendBufferSize;
	float TimeBetweenTicks;

	bool PacketSent = false;

	TWeakObjectPtr<ASocketCommunication_Cpp> SocketOwner;

	FThreadSafeBool bConnected = false;
	FThreadSafeBool bRun = false;

	// SPEC : Single producer, single consumer
	TQueue<TArray<FPacketData>, EQueueMode::Spsc> Inbox;
	TQueue<FPacketData, EQueueMode::Spsc> Outbox;

public:

	// Constructor
	FTcpSocket();
	FTcpSocket(FString inIp, const int32 inPort, TWeakObjectPtr<ASocketCommunication_Cpp> InOwner, int32 inId, int32 inRecvBufferSize, int32 inSendBufferSize, float inTimeBetweenTicks);

	// Destructor
	virtual ~FTcpSocket();

	// Starts processing of the connection. Need to be called after construction
	void Start();

	// Begin FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	// SHuts down the thread
	void SocketShutdown();

	// bConnected Getter method
	bool isConnected();

	// Adds message to outgoing message queue
	void AddToOutbox(FPacketData Message);

	// Reads a message from inbox queue
	TArray<FPacketData> ReadFromInbox();
};
