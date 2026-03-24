// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BBBHealthComponent.generated.h"

// 사망 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AActor*, Killed, AActor*, Killer);

// 체력 변경 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHPChangedSignature, float, CurrentHealth, float, MaxHealth, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BBB_API UBBBHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBBBHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
    //체력
    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Health")
    float MaxHP;

    UPROPERTY(Transient, BlueprintReadOnly, Category = "Health")
    float CurrentHP;

    // 사망
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool bIsDead;

    //이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnDeathSignature OnDeath;

    UPROPERTY(BlueprintAssignable, Category = "Health")
    FOnHPChangedSignature OnHPChanged;

    // 데미지
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeDamage(float DamageAmount, AActor* DamageCauser);

    // 회복
    UFUNCTION(BlueprintCallable, Category = "Health")
    void Heal(float HealAmount);

    // 디버프 보너스 데미지 계산
    float CalculateFinalDamage(float BaseDamage);

    // 체력 비율
    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHPPercent() const;

    // 생존 여부
    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsAlive() const { return !bIsDead; }
		
};
