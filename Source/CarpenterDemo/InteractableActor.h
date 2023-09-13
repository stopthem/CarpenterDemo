// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "InteractableActor.generated.h"

/*
 * AInteractableActor
 *
 * Class that all actors that a c++ class needs to call its interactable functions have 
 */
UCLASS()
class CARPENTERDEMO_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AInteractableActor() = default;
};
