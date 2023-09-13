// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterDemoGameMode.h"
#include "CarpenterDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACarpenterDemoGameMode::ACarpenterDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
