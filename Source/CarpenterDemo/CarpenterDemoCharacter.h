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
	/** Resets HMD orientation in VR. */
	void OnResetVR();

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

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

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
	UPROPERTY()
	AItem* CarriedItem = nullptr;

public:
	UFUNCTION(BlueprintCallable)
	AItem* GetItem() const { return CarriedItem; }

	// Called by deliver order table to get the item we are holding
	UFUNCTION(BlueprintCallable)
	AItem* DeliverItem();

	// Called when we picked up a freshly constructed item
	// Attaches the given item to our scene component
	bool TryPickupItem(AItem* Item);
};
