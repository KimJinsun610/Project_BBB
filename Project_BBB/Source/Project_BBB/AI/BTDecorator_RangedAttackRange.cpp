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
    bool bCurrentResult = CalculateRawConditionValue(OwnerComp, NodeMemory);

    // 이전 조건값과 비교해서 변했을 때만 RequestExecution
    if (bCurrentResult != bLastResult)
    {
        bLastResult = bCurrentResult;
        OwnerComp.RequestExecution(this);
    }
}
