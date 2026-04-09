// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BBBRangedAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBRangedAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABBBRangedAIController();

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
