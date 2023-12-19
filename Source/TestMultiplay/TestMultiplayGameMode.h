// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestMultiplayGameMode.generated.h"

UCLASS(minimalapi)
class ATestMultiplayGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestMultiplayGameMode();

	UFUNCTION(BlueprintCallable)
	void HostLANGame();
	UFUNCTION(BlueprintCallable)
	void JointLANGame();



};



