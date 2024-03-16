// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CarpenterDemo/Interfaces/InteractableInterface.h"
#include "InteractorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CARPENTERDEMO_API IInteractorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Lel(const TArray<FInteractInfo>& InteractInfo);

	virtual void Lel_Implementation(const TArray<FInteractInfo>& InteractInfo){}

public:
};
