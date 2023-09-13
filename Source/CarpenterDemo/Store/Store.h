// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Store.generated.h"

class UItemShapeDataAsset;
class UItemColorDataAsset;

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

private:
	UPROPERTY(EditAnywhere, Category="Orderable Items")
	TArray<UItemColorDataAsset*> OrderableColors;

public:
	UFUNCTION(BlueprintCallable)
	TArray<UItemColorDataAsset*> GetOrderableColors() { return OrderableColors; }

private:
	UPROPERTY(EditAnywhere, Category="Orderable Items")
	TArray<UItemShapeDataAsset*> OrderableShapes;

public:
	UFUNCTION(BlueprintCallable)
	TArray<UItemShapeDataAsset*> GetOrderableShapes() { return OrderableShapes; }

public:
	UPROPERTY(EditAnywhere, Category="Order Reward", meta=(UIMin="0"))
	float OrderReward = 100.0f;
};
