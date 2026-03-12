// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BBBCharacterBase.h"
#include "BBBCharacterChick.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBCharacterChick : public ABBBCharacterBase
{
	GENERATED_BODY()
public:
	ABBBCharacterChick();

protected:
	virtual void SetupCharacterMesh() override;

};
