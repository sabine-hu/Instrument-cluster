/**
 *
 *		@file		InstrumentCluster_Cpp.h
 *  	@author 	Pravaig Dynamics HMI Software Development Team
 *  	@date 		09-Apr-2022
 *  	@brief		Header file for Instrument cluster. Vehicle variables integrated with UI Widget components
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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "SocketCommunication_Cpp.h"
#include "InstrumentCluster_Cpp.generated.h"


UCLASS()
class CONSOLEHMI_API AInstrumentCluster_Cpp : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInstrumentCluster_Cpp();

	// Variable to store Timer handler for start packet processing and continuous packet processing
	UPROPERTY()
		FTimerHandle TriggerStartTimerHandler;

	// Variable to store the reference of Socket communication module
	UPROPERTY(EditAnywhere, Category = "Socket Communication")
		ASocketCommunication_Cpp* Socket;

	// Variable to store the theme of Instrument Cluster
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Data")
		int VehicleTheme = 0;

	// Variable to store speed of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Data")
		int VehicleSpeed = 1;

	// Varibale to store range of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Data")
		int VehicleRange = 1;

	// Variable to store Vehicle Gear ( 0 -> P, 1 -> R, 2 -> N, 3 -> D )
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Data")
		uint8 VehicleGear = 0;

	// Variable to store ambient temperature of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Data")
		float VehicleAmbientTemperature = 27;

	// Variable to store battery volatage of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Battery Data")
		float VehicleBatteryVoltage = 0.0f;

	// Variable to store vehicle Battery SoC
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Battery Data")
		float VehicleBatterySoC = 0.0f;

	// Variable to show vehicle battery charge status
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Battery Data")
		bool VehicleBatteryChargeStatus = false;

	// Variable to store temperature of the motor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Motor Data")
		float VehicleMotorTemperature = 194.0f;

	// Variable to store left indicator value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Indicators Data")
		bool VehicleLeftIndicator = false;

	// Variable to store right indicator value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Indicators Data")
		bool VehicleRightIndicator = false;


	// Variable to store front fog lamp value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Lighting and Signaling Data")
		bool VehicleFrontFogLamp = false;

	// Variable to store rear fog lamp value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Lighting and Signaling Data")
		bool VehicleRearFogLamp = false;

	// Variable to store high beam head lamp value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Lighting and Signaling Data")
		bool VehicleHighBeamHeadLamp = false;

	// Variable to store dipped beam head lamp value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Lighting and Signaling Data")
		bool VehicleDippedBeamHeadLamp = false;

	// Variable to store position lamp value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Lighting and Signaling Data")
		bool VehiclePositionLamp = false;

	// Variable to store parking lamp value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Lighting and Signaling Data")
		bool VehicleParkingLamp = false;

	// Variable to store ABS malfunction value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Malfunctioning Data")
		bool VehicleABSMalfunction = false;

	// Variable to store brake malfunction value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Malfunctioning Data")
		bool VehicleBrakeMalfunction = false;

	// Variable to store worn brake lining malfunction value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Malfunctioning Data")
		bool VehicleWornBrakeLiningMalfunction = false;

	// Variable to store hood open/close value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Exterior Data")
		bool VehicleHoodState = false;

	// Variable to store Sunroof open/close value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Exterior Data")
		bool VehicleSunRoofState = false;

	// Variable to store front left door open/close value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Exterior Data")
		bool VehicleFrontLeftDoorState = false;

	// Variable to store front right door open/close value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Exterior Data")
		bool VehicleFrontRightDoorState = false;

	// Variable to store rear left door open/close value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Exterior Data")
		bool VehicleRearLeftDoorState = false;

	// Variable to store rear right door open/close value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Exterior Data")
		bool VehicleRearRightDoorState = false;

	// Variable to store trunk open/close value of the vehicle
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Vehicle Exterior Data")
		bool VehicleTrunkState = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Server Data")
		bool bIsCommunicating = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bUIUpdated = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bStartFirstPacketProcessing = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 ConnectionId = 0;

	// Variable to keep on check whther first packet is sent or not 
	UPROPERTY()
		bool FirstPacketSent = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to start packet processing with server
	UFUNCTION()
		void StartPacketProcessing(FPacketData PacketData);

	// Function to update the UI parameters accroding to the data received from server
	UFUNCTION()
		void UpdateUIParameters(const TArray<FPacketData>& PacketsData);

	// Function to send the initial packet to the server - A RequestAll command
	UFUNCTION()
		void SendInitialPacket();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
