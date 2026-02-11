// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BBBCharacterBase.h"
#include "MyBBBCharacterChick.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API AMyBBBCharacterChick : public ABBBCharacterBase
{
	GENERATED_BODY()

public:
	AMyBBBCharacterChick();

protected:
	virtual void SetupCharacterMesh() override;

	
};
