// Copyright Epic Games, Inc. All Rights Reserved.

/**
 *
 *		@file		ConsoleHMI.Build.cs
 *  	@author 	Pravaig Dynamics HMI Software Development Team
 *  	@date 		09-Apr-2022
 *  	@brief		File to add all the public and private dependency for the project
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

using UnrealBuildTool;

public class ConsoleHMI : ModuleRules
{
	public ConsoleHMI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "Sockets", "Networking" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		
		// Uncomment if you are using online features
		 PrivateDependencyModuleNames.Add("OnlineSubsystem");


	}
}
