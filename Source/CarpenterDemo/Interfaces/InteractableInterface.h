#pragma once

#include "InteractableInterface.generated.h"

UENUM(BlueprintType)
enum EInputKeyActionName
{
	Q,
	W,
	E
};

USTRUCT(BlueprintType)
struct FInteractInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName name = FName("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EInputKeyActionName> keyname;
};

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
