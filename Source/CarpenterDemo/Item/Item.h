// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemShapeDataAsset.h"
#include "CarpenterDemo/Interfaces/InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
enum EItemShapes : int;

/*
 * FItemInfo
 *
 * Holds information about item
 */
USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FColor ItemColor;

	UPROPERTY(BlueprintReadWrite)
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

protected:
	// Create dynamic instance material for color changes
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	// Our root component
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

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

	UFUNCTION(BlueprintCallable)
	void SetItemInfo(const FItemInfo& NewItemInfo);

public:
	// Changes ItemColor and updates the actual material
	UFUNCTION(BlueprintCallable)
	void SetColor(const FColor& NewColor);

private:
	// Updates the actual material with ItemInfo.ItemColor
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
