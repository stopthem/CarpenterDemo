// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemShapeDataAsset.h"
#include "CarpenterDemo/Interfaces/InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

enum EItemShapes : int;

/*
 * FItemInfo
 *
 * Holds information about item
 */
USTRUCT()
struct FItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FColor ItemColor;

	UPROPERTY()
	TEnumAsByte<EItemShapes> ItemShape = Sphere;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItem_OnPickedUp);

/*
 * AItem
 *
 * Class that all items have.
 */
UCLASS()
class CARPENTERDEMO_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AItem();

public:
	// Create dynamic instance material for color changes
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	// The material that we change the color of it
	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstanceDynamic;

private:
	FItemInfo ItemInfo;

public:
	FItemInfo GetItemInfo() const { return ItemInfo; }

	void SetItemInfo(const FItemInfo& NewItemInfo);

public:
	// Changes ItemColor and updates the actual material
	UFUNCTION(BlueprintCallable)
	void SetColor(const FColor& NewColor);

private:
	void UpdateColor() const;

public:
	// Handles being picked up by player
	virtual void OnInteract_Implementation(AActor* Interactor) override;

private:
	bool bPickedUp = false;

public:
	// Broadcasted when this item has been picked up
	UPROPERTY(BlueprintAssignable)
	FItem_OnPickedUp OnPickedUp;
};
