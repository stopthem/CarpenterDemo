// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemColorDataAsset.generated.h"

/**
 * UItemColorDataAsset
 *
 * Holds information orderable colors
 */
UCLASS(BlueprintType)
class CARPENTERDEMO_API UItemColorDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(HideAlphaChannel="true"), BlueprintReadOnly)
	FColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ColorName;
};
