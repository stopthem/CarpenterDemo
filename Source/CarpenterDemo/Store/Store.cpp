// Fill out your copyright notice in the Description page of Project Settings.


#include "Store.h"
#include "CarpenterDemo/Item/ItemColorDataAsset.h"
#include "Net/UnrealNetwork.h"

AStore::AStore()
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = false;
}

void AStore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AStore, CurrentBudget);
	DOREPLIFETIME(AStore, ActiveOrders);
}

void AStore::BeginPlay()
{
	Super::BeginPlay();

	CurrentBudget = StartingBudget;

	// We want to call RequestOrder() only from server actor
	// So if not server actor, return
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	// Waiting a tick because there are objects that bind to OnOrderRequested at BeginPlay()
	GetWorldTimerManager().SetTimerForNextTick(this, &AStore::RequestOrder);
}

void AStore::Nmc_BroadcastOnOrderRequested_Implementation(const FOrderInfo OrderInfo)
{
	OnOrderRequested.Broadcast(OrderInfo);
}

void AStore::RequestOrder()
{
	// Get random color data asset
	UItemColorDataAsset* RandomColorDataAsset = OrderableColorDataAssets[FMath::RandRange(0, OrderableColorDataAssets.Num() - 1)];

	// Get random shape asset
	UItemShapeDataAsset* RandomShapeAsset = OrderableShapeDataAssets[FMath::RandRange(0, OrderableShapeDataAssets.Num() - 1)];

	const FOrderInfo RequestedOrderInfo = {RandomColorDataAsset, RandomShapeAsset};

	ActiveOrders.Add(RequestedOrderInfo);

	// We want all clients to be notified that a order has been requested
	Nmc_BroadcastOnOrderRequested(RequestedOrderInfo);
}

void AStore::OnConstructedItemPickedUp()
{
	FTimerHandle TimerHandle;
	// We wait a random time between NewOrderWaitTimeMin and NewOrderWaitTimeMax for the next order request
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AStore::RequestOrder, FMath::RandRange(NewOrderWaitTimeMin, NewOrderWaitTimeMax), false);
}

void AStore::CollectOrder(const AItem* Item)
{
	int32 Reward = OrderReward;

	// We want the oldest added order
	const FOrderInfo OrderInfo = ActiveOrders[0];
	ActiveOrders.RemoveAt(0);

	const FItemInfo ItemInfo = Item->GetItemInfo();

	// Notify all clients and server that a order has been collected
	Nmc_BroadcastOnOrderCollected();

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
}

void AStore::Nmc_BroadcastOnOrderCollected_Implementation()
{
	OnOrderCollected.Broadcast();
}

void AStore::SpendBudget(const int Amount)
{
	CurrentBudget -= Amount;
	CurrentBudget = FMath::Clamp(CurrentBudget, 0, INT_MAX);
}
