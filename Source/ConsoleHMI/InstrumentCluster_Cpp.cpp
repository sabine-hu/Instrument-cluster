/**
 *
 *		@file		InstrumentCluster_Cpp.cpp
 *  	@author 	Pravaig Dynamics HMI Software Development Team
 *  	@date 		09-Apr-2022
 *  	@brief		UI Widget integration class. Vehicle values gets directly reflected in the UI
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

#include "InstrumentCluster_Cpp.h"
#include "SocketCommunication_Cpp.h"


// Sets default values
AInstrumentCluster_Cpp::AInstrumentCluster_Cpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInstrumentCluster_Cpp::BeginPlay()
{
	Super::BeginPlay();
	Socket->Connect("127.0.0.1", 7777, ConnectionId);
	GetWorld()->GetTimerManager().SetTimer(TriggerStartTimerHandler, this, &AInstrumentCluster_Cpp::SendInitialPacket, 3.9f, false);

}

void AInstrumentCluster_Cpp::SendInitialPacket()
{
	bStartFirstPacketProcessing = true;

	//UE_LOG(LogTemp, Warning, TEXT("Instrument Cluster_Cpp : SendInitialPacket"));

}

void AInstrumentCluster_Cpp::StartPacketProcessing(FPacketData PacketData)
{


}

void AInstrumentCluster_Cpp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(TriggerStartTimerHandler);

}

// Called every frame
void AInstrumentCluster_Cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStartFirstPacketProcessing)
	{
		FPacketData FirstRequestAllPacket;

		FirstRequestAllPacket.Command = E_VCommand::EV_RequestAll;
		FirstRequestAllPacket.Status = E_VStatus::EV_VS_Gear;
		FirstRequestAllPacket.Data = 0;

		Socket->SendData(ConnectionId, FirstRequestAllPacket);

		//UE_LOG(LogTemp, Warning, TEXT("Initial Packet Sent!!"));

		bStartFirstPacketProcessing = false;
	}


	if (FirstPacketSent && bUIUpdated)
	{
		bUIUpdated = false;
		bIsCommunicating = true;


		//UE_LOG(LogTemp, Warning, TEXT("Sending Packet"));
		FPacketData UpdatePacket;
		UpdatePacket.Command = E_VCommand::EV_Updated;
		UpdatePacket.Status = E_VStatus::EV_VS_Gear;
		UpdatePacket.Data = 0;

		//UE_LOG(LogTemp, Warning, TEXT("Continuous Packet Sent"));

		Socket->SendData(ConnectionId, UpdatePacket);


	}

	UpdateUIParameters(Socket->ReceiveData(ConnectionId));

}



void AInstrumentCluster_Cpp::UpdateUIParameters(const TArray<FPacketData>& PacketsReceived)
{
	// Log


	if (PacketsReceived.Num() > 0)
	{
		for (int i = 0; i < PacketsReceived.Num(); i++)
		{
			if (PacketsReceived[i].Command == E_VCommand::EV_Data)
			{
				switch (PacketsReceived[i].Status)
				{

				case E_VStatus::EV_VS_MotorTemerature: {
					VehicleMotorTemperature = (float)(*((float*)&PacketsReceived[i].Data));

					break;
				}

				case E_VStatus::EV_VS_LeftIndicator: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleLeftIndicator = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleLeftIndicator = true;

					break;
				}

				case E_VStatus::EV_VS_RightIndicator: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleRightIndicator = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleRightIndicator = true;

					break;
				}

				case E_VStatus::EV_VS_Speed: {
					VehicleSpeed = (int)(*((uint32*)&PacketsReceived[i].Data));

					break;
				}


				case E_VStatus::EV_VS_Range: {
					VehicleRange = (int)(*((uint32*)&PacketsReceived[i].Data));

					break;
				}

				case E_VStatus::EV_VS_Gear: {
					VehicleGear = (int)(*((uint32*)&PacketsReceived[i].Data));

					break;
				}

				case E_VStatus::EV_VS_AmbientTemperature: {
					VehicleAmbientTemperature = (float)(*((float*)&PacketsReceived[i].Data));

					break;
				}

				case E_VStatus::EV_VS_BatterySOC: {
					VehicleBatterySoC = (float)(*((float*)&PacketsReceived[i].Data));

					break;
				}

				case E_VStatus::EV_VS_BatteryChargeStatus: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleBatteryChargeStatus = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleBatteryChargeStatus = true;

					break;
				}

				case E_VStatus::EV_VS_FrontFogLamp: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleFrontFogLamp = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleFrontFogLamp = true;

					break;
				}

				case E_VStatus::EV_VS_RearFogLamp: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleRearFogLamp = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleRearFogLamp = true;

					break;
				}

				case E_VStatus::EV_VS_HighBeamHeadLamp: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleHighBeamHeadLamp = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleHighBeamHeadLamp = true;

					break;
				}

				case E_VStatus::EV_VS_DippedBeamHeadLamp: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleDippedBeamHeadLamp = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleDippedBeamHeadLamp = true;

					break;
				}

				case E_VStatus::EV_VS_PositionLamp: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehiclePositionLamp = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehiclePositionLamp = true;

					break;
				}

				case E_VStatus::EV_VS_ParkingLamp: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleParkingLamp = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleParkingLamp = true;

					break;
				}

				case E_VStatus::EV_VS_AbsMalfunction: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleABSMalfunction = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleABSMalfunction = true;

					break;
				}

				case E_VStatus::EV_VS_BrakeMalfunction: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleBrakeMalfunction = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleBrakeMalfunction = true;

					break;
				}

				case E_VStatus::EV_VS_WornBrakeLiningMalfunction: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleWornBrakeLiningMalfunction = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleWornBrakeLiningMalfunction = true;

					break;
				}

				case E_VStatus::EV_VS_HoodState: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleHoodState = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleHoodState = true;

					break;
				}

				case E_VStatus::EV_VS_SunRoofState: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleSunRoofState = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleSunRoofState = true;

					break;
				}

				case E_VStatus::EV_VS_FrontLeftDoorState: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleFrontLeftDoorState = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleFrontLeftDoorState = true;

					break;
				}

				case E_VStatus::EV_VS_FrontRightDoorState: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleFrontRightDoorState = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleFrontRightDoorState = true;

					break;
				}

				case E_VStatus::EV_VS_RearLeftDoorState: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleRearLeftDoorState = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleRearLeftDoorState = true;

					break;
				}

				case E_VStatus::EV_VS_RearRightDoorState: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleRearRightDoorState = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleRearRightDoorState = true;

					break;
				}

				case E_VStatus::EV_VS_TrunkState: {
					if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 0)
						VehicleTrunkState = false;
					else if ((int)(*((uint32*)&PacketsReceived[i].Data)) == 1)
						VehicleTrunkState = true;

					break;
				}


				default:
					break;

				}
			}
		}
	
		FirstPacketSent = true;
		bUIUpdated = true;
		bIsCommunicating = false;
	}

}




