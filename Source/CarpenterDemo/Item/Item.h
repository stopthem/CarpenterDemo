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
	FColor ItemColor = FColor::Silver;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EItemShapes> ItemShape = None;
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
	// Replicate variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Sphere component that other objects overlap to interact us 
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	// Our root component
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

#pragma region Item Info

private:
	// Our color and shape data
	UPROPERTY(ReplicatedUsing = OnRep_ItemInfo)
	FItemInfo ItemInfo;

	// Update color when item info changed and replicated to all instances
	UFUNCTION()
	void OnRep_ItemInfo();

public:
	UFUNCTION(BlueprintCallable)
	FItemInfo GetItemInfo() const { return ItemInfo; }

	UFUNCTION(BlueprintCallable)
	void SetItemInfo(const FItemInfo& NewItemInfo);
#pragma endregion

#pragma region Color

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	// Our main material that we will change its color
	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstanceDynamic;

public:
	// Changes material color
	UFUNCTION(BlueprintCallable)
	void SetColor(const FColor& NewColor);

private:
	// Updates the actual material with ItemInfo
	void UpdateColor();
#pragma endregion

#pragma region Picked Up

public:
	void PickedUp();

public:
	// Handles being picked up by player
	virtual void OnInteract_Implementation(AActor* Interactor) override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_bPickedUp)
	bool bPickedUp = false;

	UFUNCTION()
	void OnRep_bPickedUp();

public:
	bool IsPickedUp() const { return bPickedUp; }

public:
	// Broadcasted when this item has been picked up
	UPROPERTY(BlueprintAssignable)
	FItem_OnPickedUp OnPickedUp;
#pragma endregion
};
