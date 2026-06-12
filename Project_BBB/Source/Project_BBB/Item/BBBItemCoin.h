// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/BBBItemBase.h"
#include "BBBItemCoin.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBItemCoin : public ABBBItemBase
{
	GENERATED_BODY()

public:
	ABBBItemCoin();

	virtual void OnPickup_Implementation(AActor* Picker) override;


	
};
