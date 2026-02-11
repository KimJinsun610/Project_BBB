// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BBBCharacterBase.h"
#include "BBBCharacterBat.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBCharacterBat : public ABBBCharacterBase
{
	GENERATED_BODY()
	
public:
	ABBBCharacterBat();

protected:
	virtual void SetupCharacterMesh() override;
};
