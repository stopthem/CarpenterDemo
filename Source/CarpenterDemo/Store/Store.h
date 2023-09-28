// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarpenterDemo/Item/Item.h"
#include "GameFramework/Actor.h"

#include "Store.generated.h"


class ACarpenterDemoGameMode;
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

#pragma region Request Order

private:
	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s", ClampMin="0"))
	float NewOrderWaitTimeMin = 2.0f;

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s", ClampMin="0"))
	float NewOrderWaitTimeMax = 4.0f;

public:
	// Currently active orders
	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<FOrderInfo> ActiveOrders;

private:
	// Broadcasted when a order has been requested from server
	UPROPERTY(BlueprintAssignable)
	FStore_OnOrderRequested OnOrderRequested;

	// Request a new random order
	// Only executed in server instance of this actor
	UFUNCTION()
	void RequestOrder();

public:
	// Broadcast OnOrderRequested on all Clients and Server
	UFUNCTION(NetMulticast, Reliable)
	void Nmc_BroadcastOnOrderRequested(const FOrderInfo OrderInfo);

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
#pragma endregion

public:
	// Called when ChippingTable constructed item is picked up so we request a new order
	// Only executed in server instance of this actor
	UFUNCTION(BlueprintCallable)
	void OnConstructedItemPickedUp();

#pragma region Order Collected

public:
	// Called when OrderDeliverTable collected a item locally
	// Only executed in server instance of this actor
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
	UFUNCTION(NetMulticast, Reliable)
	void Nmc_BroadcastOnOrderCollected();
#pragma endregion

#pragma region Budget

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 CurrentBudget;

private:
	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin="0"))
	int32 StartingBudget = 500;

public:
	// Reduce budget by Amount
	// Only executed in server instance of this actor
	UFUNCTION(BlueprintCallable)
	void SpendBudget(int Amount);
#pragma endregion
};
