// Fill out your copyright notice in the Description page of Project Settings.


#include "OrderInfoBlueprintFunctionLibrary.h"

#include "CarpenterDemo/Store/Store.h"

bool UOrderInfoBlueprintFunctionLibrary::CompareOrderInfos(const FOrderInfo& OrderInfo, const FOrderInfo& OtherOrderInfo)
{
	return OrderInfo == OtherOrderInfo;
}
