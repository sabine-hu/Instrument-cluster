// Copyright Epic Games, Inc. All Rights Reserved.

/**
 *
 *		@file		ConsoleHMIGameModeBase.h
 *  	@author 	Pravaig Dynamics HMI Software Development Team
 *  	@date 		09-Apr-2022
 *  	@brief		Project default game mode base class
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
#include "GameFramework/GameModeBase.h"
#include "ConsoleHMIGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CONSOLEHMI_API AConsoleHMIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HMIs", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> PlayerHUDClass;

		UPROPERTY()
		class UUserWidget* CurrentWidget;
	
};
