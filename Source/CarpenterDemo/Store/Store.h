// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarpenterDemo/Item/Item.h"
#include "GameFramework/Actor.h"

#include "Store.generated.h"


class ACarpenterDemoGameMode;
class UKismetStringLibrary;
class AItem;
class UItemShapeDataAsset;
class UItemColorDataAsset;

/*
 * FOrderInfo
 *
 * Struct that holds a order
 */
USTRUCT(BlueprintType)
struct FOrderInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Order")
	UItemColorDataAsset* ItemColorDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Order")
	UItemShapeDataAsset* ItemShapeDataAsset = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStore_OnOrderRequested, FOrderInfo, OrderInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStore_OnOrderCollected);

/*
 * AStore
 *
 * Handles all about money, orders and items
 */
UCLASS()
class CARPENTERDEMO_API AStore : public AActor
{
	GENERATED_BODY()

public:
	AStore();

protected:
	// Call GameMode to request a store order
	virtual void BeginPlay() override;

protected:
	// Replicate variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	ACarpenterDemoGameMode* CarpenterDemoGameMode;

private:
	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s"))
	float NewOrderWaitTimeMin = 2.0f;

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s"))
	float NewOrderWaitTimeMax = 4.0f;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<FOrderInfo> ActiveOrders;

private:
	UPROPERTY(BlueprintAssignable)
	FStore_OnOrderRequested OnOrderRequested;

public:
	// Broadcast OnOrderRequested on all Clients and Server
	UFUNCTION(NetMulticast, Unreliable)
	void Nmc_BroadcastOnOrderRequested(const FOrderInfo OrderInfo);

public:
	// Called when ChippingTable constructed item is picked up
	UFUNCTION(BlueprintCallable)
	void OnOrderPickedUp();

private:
	UPROPERTY(EditAnywhere, Category="Order|Orderable Items")
	TArray<UItemColorDataAsset*> OrderableColorDataAssets;

public:
	UFUNCTION()
	TArray<UItemColorDataAsset*> GetOrderableColorDataAssets() const { return OrderableColorDataAssets; }

private:
	UPROPERTY(EditAnywhere, Category="Order|Orderable Items")
	TArray<UItemShapeDataAsset*> OrderableShapeDataAssets;

public:
	UFUNCTION(BlueprintCallable)
	TArray<UItemShapeDataAsset*> GetOrderableShapeDataAssets() { return OrderableShapeDataAssets; }

public:
	// Called when OrderDeliverTable collected a item locally
	UFUNCTION(BlueprintCallable)
	void CollectOrder(const AItem* Item);

	// Reward for perfect order deliver
	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin="0"))
	int32 OrderReward = 100;

private:
	UPROPERTY(BlueprintAssignable)
	FStore_OnOrderCollected OnOrderCollected;

public:
	// Broadcast OnOrderCollected to all Clients and Server
	UFUNCTION(NetMulticast, Unreliable)
	void Nmc_BroadcastOnOrderCollected();

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 CurrentBudget;

private:
	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin="0"))
	int32 StartingBudget = 500;

public:
	UFUNCTION(BlueprintCallable)
	void SpendBudget(int Amount);
	
	void SpendBudget_Implementation(int Amount);
};
