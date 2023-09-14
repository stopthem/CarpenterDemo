// Fill out your copyright notice in the Description page of Project Settings.


#include "Store.h"

#include "CarpenterDemo/Item/ItemColorDataAsset.h"
#include "Net/UnrealNetwork.h"

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

void AStore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStore, CurrentBudget);
}

void AStore::RequestOrder_Implementation()
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

void AStore::CollectOrder_Implementation(const AItem* Item)
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

	CurrentBudget += Reward;

	OnOrderCollected.Broadcast(OrderInfo);
}

void AStore::SpendBudget_Implementation(int Amount)
{
	CurrentBudget -= Amount;
	CurrentBudget = FMath::Clamp(CurrentBudget, 0,INT_MAX);
}
