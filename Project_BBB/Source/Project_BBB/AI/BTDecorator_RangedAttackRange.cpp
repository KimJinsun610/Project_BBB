// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_RangedAttackRange.h"
#include "BTDecorator_RangedAttackRange.h"

UBTDecorator_RangedAttackRange::UBTDecorator_RangedAttackRange()
{
    NodeName = TEXT("CanAttack_Ranged");
    bNotifyTick = true;
}


void UBTDecorator_RangedAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    OwnerComp.RequestExecution(this);
}
