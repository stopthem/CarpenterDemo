#pragma once

#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class IInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interfaces|InteractableInterface")
	void OnInteract(AActor* Interactor);

	virtual void OnInteract_Implementation(AActor* Interactor) {}

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interfaces|InteractableInterface")
	void OnInteractEnd(AActor* Interactor);

	virtual void OnInteractEnd_Implementation(AActor* Interactor) {}
};
