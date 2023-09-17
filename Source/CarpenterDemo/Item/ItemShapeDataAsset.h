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
	None UMETA(Hidden), // Added because if struct values are not changed in ItemInfo, struct does not call OnRep.
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
	TEnumAsByte<EItemShapes> ItemShape = Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AItem> ItemBP = nullptr;
};
