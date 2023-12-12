// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestMultiplayGameMode.h"
#include "TestMultiplayCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestMultiplayGameMode::ATestMultiplayGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATestMultiplayGameMode::HostLANGame()
{
	GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

void ATestMultiplayGameMode::JointLANGame()
{
	APlayerController *PC = GetGameInstance()->GetFirstLocalPlayerController();

	if (PC)
	{
		PC->ClientTravel("192.168.50.20", TRAVEL_Absolute);
	}
}
