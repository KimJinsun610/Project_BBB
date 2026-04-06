// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/BBBCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Combat/BBBDebuffTypes.h"     
#include "AI/BBBCharacterAIInterface.h"
#include "Item/BBBItemDataAsset.h"

#include "BBBEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBEnemyBase : public ABBBCharacterBase, public IBBBCharacterAIInterface
{
	GENERATED_BODY()

public:
	ABBBEnemyBase();

protected:
	bool RangedEnemy = 0; //true == 원거리

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

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateEnemyDebuff();

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

	// 디버프 변화 콜백
	UFUNCTION()
	void OnDebuffChangeCallback(EDebuffType DebuffType);

//======================================================
// Attack Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> MeleeAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float AttackRange;

	void PerformAttack();

	void PerformMeleeAttack();
	void MeleeAttackBegin();
	void MeleeAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

//======================================================
//AI section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

//======================================================
//Item Section
protected:
	// 드롭 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drop")
	TArray<FItemDropData> DropTable;

	void DropItems(); // 아이템 드롭 함수

	UFUNCTION()
	void OnEnemyDeath(AActor* Killed, AActor* Killer);
};
