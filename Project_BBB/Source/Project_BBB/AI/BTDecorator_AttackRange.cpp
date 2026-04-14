// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "BBBAI.h"
#include "BBBCharacterAIInterface.h"

#include "DrawDebugHelpers.h"

UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IBBBCharacterAIInterface* AIPawn = Cast<IBBBCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);

	/*
	// 디버그 원
	DrawDebugSphere(
		ControllingPawn->GetWorld(),
		ControllingPawn->GetActorLocation(),
		AttackRangeWithRadius,
		16,
		bResult ? FColor::Yellow : FColor::Orange, // 범위 안 = 노랑, 밖 = 주황
		false,
		0.1f
	);
	*/
	return bResult;
}

