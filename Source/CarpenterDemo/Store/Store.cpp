// Fill out your copyright notice in the Description page of Project Settings.


#include "Store.h"

#include "CarpenterDemo/CarpenterDemoGameMode.h"
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

	// Waiting a tick because chipping table widget subscribes to request order event at begin play
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]
	{
		AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode();

		// Only server actor has the game mode
		if (!GameModeBase)
		{
			return;
		}

		CarpenterDemoGameMode = Cast<ACarpenterDemoGameMode>(GameModeBase);
		CarpenterDemoGameMode->Store_RequestOrder(this);
	}));
}

void AStore::Nmc_BroadcastOnOrderRequested_Implementation(const FOrderInfo OrderInfo)
{
	OnOrderRequested.Broadcast(OrderInfo);
}

void AStore::OnOrderPickedUp()
{
	// Create a random timed timer for next order request 
	FTimerHandle TimerHandle;
	// We wait a random time between NewOrderWaitTimeMin and NewOrderWaitTimeMax for the next order request
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		// Only server actor has the game mode
		if (!CarpenterDemoGameMode)
		{
			return;
		}

		CarpenterDemoGameMode->Store_RequestOrder(this);
	}), FMath::RandRange(NewOrderWaitTimeMin, NewOrderWaitTimeMax), false);
}

void AStore::CollectOrder(const AItem* Item)
{
	// Only server actor has the game mode
	if (!CarpenterDemoGameMode)
	{
		return;
	}

	CarpenterDemoGameMode->Store_CollectOrder(this, Item, OrderReward);
}

void AStore::Nmc_BroadcastOnOrderCollected_Implementation()
{
	OnOrderCollected.Broadcast();
}

void AStore::SpendBudget(const int Amount)
{
	CurrentBudget -= Amount;
	CurrentBudget = FMath::Clamp(CurrentBudget, 0,INT_MAX);
}
