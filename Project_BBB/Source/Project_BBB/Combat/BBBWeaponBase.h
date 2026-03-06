// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBBWeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Ranged      UMETA(DisplayName = "Ranged"),
	Melee       UMETA(DisplayName = "Melee")
};


UCLASS()
class PROJECT_BBB_API ABBBWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABBBWeaponBase();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float FireRate;  // 초당 공격 속도

    //가상 함수
    virtual void Attack() PURE_VIRTUAL(ABBBWeaponBase::Attack, );
    virtual void StopAttack() PURE_VIRTUAL(ABBBWeaponBase::StopAttack, );

    // 장착/해제
    void Equip(ACharacter* Character);
    void Unequip();

    // 원거리용 발사 확인
    bool bCanFire = false;

protected:
    UPROPERTY()
    ACharacter* OwnerCharacter;

    bool bCanAttack;
    FTimerHandle AttackCooldownTimer;

protected:
	virtual void BeginPlay() override;

};
