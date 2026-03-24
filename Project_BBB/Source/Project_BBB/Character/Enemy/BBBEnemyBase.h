// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BBBCharacterBase.h"
#include "BBBEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBEnemyBase : public ABBBCharacterBase
{
	GENERATED_BODY()

public:
	ABBBEnemyBase();

protected:
	virtual void SetupCharacterMesh() override;
	
};
