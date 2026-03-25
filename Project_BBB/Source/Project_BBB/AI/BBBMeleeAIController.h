// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BBBMeleeAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBMeleeAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABBBMeleeAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
