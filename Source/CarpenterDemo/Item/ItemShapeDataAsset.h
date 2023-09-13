// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemShapeDataAsset.generated.h"

class AItem;

/*
 * EItemShapes
 *
 * Enum about shapes that can be ordered from store
 */
UENUM(BlueprintType)
enum EItemShapes
{
	Sphere,
	Cube
};

/**
 * UItemShapeDataAsset
 *
 * Holds information about orderable item shapes
 */
UCLASS(BlueprintType)
class CARPENTERDEMO_API UItemShapeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EItemShapes> ItemShape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AItem> ItemBP = nullptr;
};
