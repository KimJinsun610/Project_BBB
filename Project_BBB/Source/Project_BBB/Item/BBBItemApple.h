// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BBBItemBase.h"
#include "BBBItemApple.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBItemApple : public ABBBItemBase
{
	GENERATED_BODY()
	
public:
	ABBBItemApple();

	virtual void OnPickup_Implementation(AActor* Picker) override;
};
