// Fill out your copyright notice in the Description page of Project Settings.


#include "Store.h"

#include "CarpenterDemo/Item/ItemColorDataAsset.h"

bool FOrderInfo::operator==(const FOrderInfo& OtherOrderInfo) const
{
	return ItemColorDataAsset == OtherOrderInfo.ItemColorDataAsset && ItemShapeDataAsset == OtherOrderInfo.ItemShapeDataAsset;
}

AStore::AStore()
{
}

void AStore::BeginPlay()
{
	Super::BeginPlay();

	CurrentBudget = StartingBudget;

	// Waiting a tick because chipping table widget subscribes to request order event at begin play
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]
	{
		RequestOrder();
	}));
}

void AStore::RequestOrder()
{
	// Get random color data asset
	UItemColorDataAsset* RandomColorDataAsset = OrderableColors[FMath::RandRange(0, OrderableColors.Num() - 1)];

	// Get random shape asset
	UItemShapeDataAsset* RandomShapeAsset = OrderableShapes[FMath::RandRange(0, OrderableShapes.Num() - 1)];

	const FOrderInfo OrderInfo = {RandomColorDataAsset, RandomShapeAsset};

	OnOrderRequested.Broadcast(OrderInfo);

	ActiveOrders.Enqueue(OrderInfo);
}

void AStore::OnOrderPickedUp()
{
	// Create a random timed timer for next order request 
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AStore::RequestOrder, FMath::RandRange(NewOrderWaitTimeMin, NewOrderWaitTimeMax));
}

void AStore::CollectOrder(const AItem* Item)
{
	int32 Reward = OrderReward;

	FOrderInfo OrderInfo;
	ActiveOrders.Dequeue(OrderInfo);

	const FItemInfo ItemInfo = Item->GetItemInfo();
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

	CurrentMoney += Reward;

	OnOrderCollected.Broadcast(OrderInfo);
}

void AStore::SpendBudget(int Amount)
{
	CurrentBudget -= Amount;
}
