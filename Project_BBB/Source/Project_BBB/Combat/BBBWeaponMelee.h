// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/BBBWeaponBase.h"
#include "BBBWeaponMelee.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBWeaponMelee : public ABBBWeaponBase
{
	GENERATED_BODY()
public:
    ABBBWeaponMelee();

    virtual void Attack() override;
    virtual void StopAttack() override;

protected:
    UPROPERTY(EditAnywhere, Category = "Weapon Stats")
    float Damage;

    UPROPERTY(EditAnywhere, Category = "Weapon Stats")
    float AttackRange;

    // 충돌 박스
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* HitBox;

    UFUNCTION()
    void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 공격 수행
    void PerformMeleeAttack();


};
