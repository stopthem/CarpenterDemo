// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterDemoGameMode.h"

#include "Item/ItemColorDataAsset.h"
#include "Item/ItemShapeDataAsset.h"
#include "Store/Store.h"
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

void ACarpenterDemoGameMode::Store_CollectOrder(AStore* Store, const AItem* Item, const int OrderReward) const
{
	int32 Reward = OrderReward;

	// Get the actual ActiveOrders array because we will change it and it needs to happen in server
	TArray<FOrderInfo>& ActiveOrders = Store->ActiveOrders;
	// We want the oldest added order
	const FOrderInfo OrderInfo = ActiveOrders[0];
	ActiveOrders.RemoveAt(0);

	const FItemInfo ItemInfo = Item->GetItemInfo();

	// Notify all clients and server that a order has been collected
	Store->Nmc_BroadcastOnOrderCollected();

	// Our shape is not the same
	if (ItemInfo.ItemShape != OrderInfo.ItemShapeDataAsset->ItemShape)
	{
		return;
	}

	// Our color is not the same
	if (ItemInfo.ItemColor != OrderInfo.ItemColorDataAsset->Color)
	{
		Reward /= 2;
	}

	Store->CurrentBudget += Reward;
}

void ACarpenterDemoGameMode::Store_RequestOrder(AStore* Store) const
{
	const TArray<UItemShapeDataAsset*> OrderableShapeDataAssets = Store->GetOrderableShapeDataAssets();
	const TArray<UItemColorDataAsset*> OrderableColorDataAssets = Store->GetOrderableColorDataAssets();

	// Get random color data asset
	UItemColorDataAsset* RandomColorDataAsset = OrderableColorDataAssets[FMath::RandRange(0, OrderableColorDataAssets.Num() - 1)];

	// Get random shape asset
	UItemShapeDataAsset* RandomShapeAsset = OrderableShapeDataAssets[FMath::RandRange(0, OrderableShapeDataAssets.Num() - 1)];

	const FOrderInfo RequestedOrderInfo = {RandomColorDataAsset, RandomShapeAsset};

	Store->ActiveOrders.Add(RequestedOrderInfo);

	// We want delegates to be netmulticasted so we call this method
	Store->Nmc_BroadcastOnOrderRequested(RequestedOrderInfo);
}
