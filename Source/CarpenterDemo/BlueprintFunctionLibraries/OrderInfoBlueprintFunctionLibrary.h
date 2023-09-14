// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CarpenterDemo/Store/Store.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OrderInfoBlueprintFunctionLibrary.generated.h"

/**
 * UOrderInfoBlueprintFunctionLibrary
 *
 * Functions about Order Infos for blueprints
 */
UCLASS()
class CARPENTERDEMO_API UOrderInfoBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool CompareOrderInfos(const FOrderInfo& OrderInfo, const FOrderInfo& OtherOrderInfo);
};
