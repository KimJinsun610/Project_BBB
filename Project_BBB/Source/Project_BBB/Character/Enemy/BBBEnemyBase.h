// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/BBBCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Combat/BBBDebuffTypes.h"     

#include "BBBEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBEnemyBase : public ABBBCharacterBase
{
	GENERATED_BODY()

public:
	ABBBEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void SetupCharacterMesh() override;
	

//======================================================
//UI section
protected:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	UWidgetComponent* EnemyInfoWidgetComponent;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateEnemyInfoWidget();

private:
	// HP 변경 콜백
	UFUNCTION()
	void OnHPChangedCallback(float CurrentHP, float MaxHP, AActor* DamageCauser);

	// 디버프 추가 콜백
	UFUNCTION()
	void OnDebuffAppliedCallback(EDebuffType DebuffType, float Duration);

	// 디버프 제거 콜백
	UFUNCTION()
	void OnDebuffRemovedCallback(EDebuffType DebuffType);
};
