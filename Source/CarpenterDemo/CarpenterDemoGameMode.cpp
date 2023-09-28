// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterDemoGameMode.h"

#include "UObject/ConstructorHelpers.h"

ACarpenterDemoGameMode::ACarpenterDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = false;
}
