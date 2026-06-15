// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_RangedAttackRange.h"
#include "AIController.h"    
#include "Character/Enemy/BBBEnemyBase.h"
#include "Combat/BBBDebuffComponent.h"
#include "Character/BBBStatComponent.h"


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

bool UBTDecorator_RangedAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // 거리 조건은 부모에서 그대로 사용
    if (!Super::CalculateRawConditionValue(OwnerComp, NodeMemory))
    {
        return false;
    }

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    ABBBEnemyBase* Enemy = Cast<ABBBEnemyBase>(ControllingPawn);
    if (!Enemy)
    {
        return false;
    }

    // HP 0 체크
    UBBBStatComponent* Stat = Enemy->GetStatComponent();
    if (Stat && Stat->CurrentHP <= 0.0f)
    {
        return false;
    }

    // 디버프 체크
    UBBBDebuffComponent* Debuff = Enemy->FindComponentByClass<UBBBDebuffComponent>();
    if (Debuff && Debuff->HasAnyDebuff())
    {
        return false;
    }

    return true;
}
