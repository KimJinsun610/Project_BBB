// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BBBComboActionAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API UBBBComboActionAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UBBBComboActionAsset();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
