// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CarpenterDemoCharacter.generated.h"

class AItem;
class IInteractableInterface;

UCLASS(config=Game)
class ACarpenterDemoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() const { return Cast<APlayerController>(GetController()); }

public:
	// It gives error when not implemented. I don't know why
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ACarpenterDemoCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// Handle interaction when input provided
	UFUNCTION()
	void CheckInteract();

private:
	UPROPERTY()
	AActor* CurrentInteractableActor = nullptr;

protected:
	// Handle interaction beginnings
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Handle interaction endings
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:
	// Our picked up item will be attached here
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ItemPickupAttachSceneComponent = nullptr;

	// Our currently carried item
	UPROPERTY(ReplicatedUsing = OnRep_CarriedItem)
	AItem* CarriedItem;

	UFUNCTION()
	void OnRep_CarriedItem();

public:
	UFUNCTION(BlueprintCallable)
	AItem* GetItem() const { return CarriedItem; }

	// Called by deliver order table to get the item we are holding
	UFUNCTION(BlueprintCallable, Server, Unreliable, WithValidation)
	void Server_DeliveredItem();

private:
	// Reset CarriedItem
	UFUNCTION(NetMulticast, Unreliable)
	void Client_DeliveredItem();

	// Do we really have a CarriedItem validation
	UFUNCTION()
	bool Server_DeliveredItem_Validate();

public:
	// Called when we picked up a freshly constructed item
	// Attaches the given item to our scene component
	UFUNCTION(Unreliable, NetMulticast)
	void Client_TryPickupItem(AItem* Item);

	UFUNCTION(Unreliable, Server, WithValidation)
	void Server_TryPickupItem(AItem* Item);

	UFUNCTION()
	bool Server_TryPickupItem_Validate(AItem* Item);
};
