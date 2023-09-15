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

	// No point clients doing this, it will be replicated
	if (!GIsServer)
	{
		return;
	}
	// Waiting a tick because chipping table widget subscribes to request order event at begin play
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]
	{
		RequestOrder();
	}));
}

void AStore::RequestOrder_Implementation()
{
	// Get random color data asset
	UItemColorDataAsset* RandomColorDataAsset = OrderableColors[FMath::RandRange(0, OrderableColors.Num() - 1)];

	// Get random shape asset
	UItemShapeDataAsset* RandomShapeAsset = OrderableShapes[FMath::RandRange(0, OrderableShapes.Num() - 1)];

	const FOrderInfo OrderInfo = {RandomColorDataAsset, RandomShapeAsset};
	ActiveOrders.Add(OrderInfo);
	// For server
	OnRep_ActiveOrders();

	FString lel = FString::Printf(TEXT("%s %s %i"), *UEnum::GetValueAsString(OrderInfo.ItemShapeDataAsset->ItemShape), *OrderInfo.ItemColorDataAsset->ColorName, ActiveOrders.Num());
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Silver, lel);
}

void AStore::OnRep_ActiveOrders()
{
	if (WantsToCollect)
	{
		WantsToCollect = false;
		return;
	}
	const FOrderInfo OrderInfo = ActiveOrders[0];
	FString lel = FString::Printf(TEXT("%s %s %i"), *UEnum::GetValueAsString(OrderInfo.ItemShapeDataAsset->ItemShape), *OrderInfo.ItemColorDataAsset->ColorName, ActiveOrders.Num());
	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Silver, lel);
	OnOrderRequested.Broadcast(ActiveOrders[0]);
}

// void AStore::OnRep_NewestAddedOrderInfo()
// {
// 	OnOrderRequested.Broadcast(NewestAddedOrderInfo);
// 	FString lel = FString::Printf(TEXT("%s %s %i"), *UEnum::GetValueAsString(NewestAddedOrderInfo.ItemShapeDataAsset->ItemShape), *NewestAddedOrderInfo.ItemColorDataAsset->ColorName, ActiveOrders.Num());
// 	GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Silver, lel);
// }
//
// void AStore::OnRep_NewestCollectedOrderInfo()
// {
// 	OnOrderCollected.Broadcast(NewestCollectedOrderInfo);
// 	ActiveOrders.Remove(NewestCollectedOrderInfo);
// }

void AStore::OnOrderPickedUp()
{
	// Create a random timed timer for next order request 
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AStore::RequestOrder, FMath::RandRange(NewOrderWaitTimeMin, NewOrderWaitTimeMax));
}

void AStore::CollectOrder_Implementation(const AItem* Item)
{
	int32 Reward = OrderReward;

	const FOrderInfo OrderInfo = ActiveOrders[0];
	ActiveOrders.RemoveAt(0);
	// For server
	WantsToCollect = true;
	OnRep_ActiveOrders();

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
}

void AStore::SpendBudget_Implementation(int Amount)
{
	CurrentBudget -= Amount;
	CurrentBudget = FMath::Clamp(CurrentBudget, 0,INT_MAX);
}
