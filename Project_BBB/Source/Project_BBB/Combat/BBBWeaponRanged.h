// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/BBBWeaponBase.h"
#include "BBBDebuffTypes.h"
#include "BBBWeaponRanged.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBWeaponRanged : public ABBBWeaponBase
{
	GENERATED_BODY()

public:
	ABBBWeaponRanged();

	virtual void Attack() override;
	virtual void StopAttack() override;

protected:
	// 발사체
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ABBBProjectileDebuff> ProjectileClass;

	// 디버프
	UPROPERTY(EditAnywhere, Category = "Weapon | Debuff")
	FDebuffData DebuffToApply;
	
	//발사 위치
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName MuzzleSocketName;

	// 투사체 발사
	void FireProjectile();

	// 발사 간격
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float FireDelay;

	bool bCanFire;
	FTimerHandle FireTimerHandle;

	void ResetFire();
};
