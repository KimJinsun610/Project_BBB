// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTDecorator_AttackRange.h"
#include "BTDecorator_RangedAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API UBTDecorator_RangedAttackRange : public UBTDecorator_AttackRange
{
	GENERATED_BODY()

public:
	UBTDecorator_RangedAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	mutable bool bLastResult = false;
};
