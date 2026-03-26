// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BBBEnemyBase.h"
#include "BBBEnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBEnemyMelee : public ABBBEnemyBase
{
	GENERATED_BODY()

public:
	ABBBEnemyMelee();

protected:
	virtual void BeginPlay() override;
	virtual void SetupCharacterMesh() override;
	
};
