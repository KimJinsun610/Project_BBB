// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BBBCharacterBase.h"
#include "BBBEnemyRanged.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBEnemyRanged : public ABBBCharacterBase
{
	GENERATED_BODY()
	
public:
	ABBBEnemyRanged();

protected:
	virtual void SetupCharacterMesh() override;
	
};
