// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BBBCharacterBase.h"
#include "BBBEnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBEnemyMelee : public ABBBCharacterBase
{
	GENERATED_BODY()

public:
	ABBBEnemyMelee();

protected:
	virtual void SetupCharacterMesh() override;

	
};
