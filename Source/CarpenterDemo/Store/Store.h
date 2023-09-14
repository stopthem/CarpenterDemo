// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarpenterDemo/Item/Item.h"
#include "GameFramework/Actor.h"
#include "Store.generated.h"


class AItem;
class UItemShapeDataAsset;
class UItemColorDataAsset;

USTRUCT(BlueprintType)
struct FOrderInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Order")
	UItemColorDataAsset* ItemColorDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Order")
	UItemShapeDataAsset* ItemShapeDataAsset;

	bool operator==(const FOrderInfo& OtherOrderInfo) const;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStore_OnOrderRequested, FOrderInfo, OrderInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStore_OnOrderCollected, FOrderInfo, OrderInfo);

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
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s"))
	float NewOrderWaitTimeMin = 2.0f;

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s"))
	float NewOrderWaitTimeMax = 4.0f;

	// Place a new order
	void RequestOrder();

	TQueue<FOrderInfo> ActiveOrders;

public:
	UPROPERTY(BlueprintAssignable)
	FStore_OnOrderRequested OnOrderRequested;

public:
	UFUNCTION(BlueprintCallable)
	void OnOrderPickedUp();

private:
	UPROPERTY(EditAnywhere, Category="Order|Orderable Items")
	TArray<UItemColorDataAsset*> OrderableColors;

private:
	UPROPERTY(EditAnywhere, Category="Order|Orderable Items")
	TArray<UItemShapeDataAsset*> OrderableShapes;

public:
	UFUNCTION(BlueprintCallable)
	TArray<UItemShapeDataAsset*> GetOrderableShapes() { return OrderableShapes; }

public:
	UFUNCTION(BlueprintCallable)
	void CollectOrder(const AItem* Item);

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin="0"))
	int32 OrderReward = 100;

	UPROPERTY(BlueprintAssignable)
	FStore_OnOrderCollected OnOrderCollected;

private:
	int32 CurrentBudget;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentBudget() const { return CurrentBudget; }

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin="0"))
	int32 StartingBudget = 500;

	UFUNCTION(BlueprintCallable)
	void SpendBudget(int Amount);

private:
	int32 CurrentMoney;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentMoney() const { return CurrentMoney; }
};
