// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarpenterDemo/Item/Item.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetStringLibrary.h"
#include "Store.generated.h"


class UKismetStringLibrary;
class AItem;
class UItemShapeDataAsset;
class UItemColorDataAsset;

USTRUCT(BlueprintType)
struct FOrderInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Order")
	UItemColorDataAsset* ItemColorDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Order")
	UItemShapeDataAsset* ItemShapeDataAsset = nullptr;

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

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s"))
	float NewOrderWaitTimeMin = 2.0f;

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin = "0", Units="s"))
	float NewOrderWaitTimeMax = 4.0f;

	// Place a new order
	UFUNCTION(Unreliable,NetMulticast)
	void RequestOrder();

	UPROPERTY(ReplicatedUsing = OnRep_ActiveOrders)
	TArray<FOrderInfo> ActiveOrders;

	UFUNCTION()
	void OnRep_ActiveOrders();

	// UPROPERTY(Replicated)
	bool WantsToCollect;

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
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast)
	void CollectOrder(const AItem* Item);

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin="0"))
	int32 OrderReward = 100;

	UPROPERTY(BlueprintAssignable)
	FStore_OnOrderCollected OnOrderCollected;

private:
	UPROPERTY(Replicated)
	int32 CurrentBudget;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentBudget() const { return CurrentBudget; }

	UPROPERTY(EditAnywhere, Category="Order", meta=(UIMin="0"))
	int32 StartingBudget = 500;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SpendBudget(int Amount);

	bool SpendBudget_Validate(int Amount) { return CurrentBudget >= Amount; }

	void SpendBudget_Implementation(int Amount);
};
