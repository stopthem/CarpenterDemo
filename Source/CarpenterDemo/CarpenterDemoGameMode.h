// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CarpenterDemoGameMode.generated.h"

class AItem;
class AStore;

/*
 * ACarpenterDemoGameMode
 *
 * Has server-only functions for AStore.
 */
UCLASS(minimalapi)
class ACarpenterDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACarpenterDemoGameMode();

public:
	// Only server has to request order. We do this because AStore is not owned by anybody
	// Does the actual logic of Requesting Order. Called from AStore(Server Actor)
	void Store_RequestOrder(AStore* Store) const;
	
	// Only server has to collect order. We do this because AStore is not owned by anybody
	// Does the actual logic of Collecting Order. Called from AStore(Server Actor)
	void Store_CollectOrder(AStore* Store, const AItem* Item, int OrderReward) const;
};
