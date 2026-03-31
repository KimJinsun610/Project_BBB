// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BBBDebuffTypes.h"
#include "BBBDebuffComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebuffApplied, EDebuffType, DebuffType, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebuffRemoved, EDebuffType, DebuffType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDebuffCountChanged, EDebuffType, DebuffType, int32, CurrentCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BBB_API UBBBDebuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBBBDebuffComponent();

    // 디버프 적용
    UFUNCTION(BlueprintCallable, Category = "Debuff")
    void ApplyDebuff(const FDebuffData& DebuffData);

    // 디버프 제거
    UFUNCTION(BlueprintCallable, Category = "Debuff")
    void RemoveDebuff(EDebuffType DebuffType);

    // 디버프 확인
    UFUNCTION(BlueprintPure, Category = "Debuff")
    bool HasDebuff(EDebuffType DebuffType) const;

    UFUNCTION(BlueprintPure, Category = "Debuff")
    bool HasAnyDebuff() const;

    // 현재 활성화된 디버프들
    UFUNCTION(BlueprintPure, Category = "Debuff")
    TArray<FDebuffData> GetActiveDebuffs() const;

    // 델리게이트
    UPROPERTY(BlueprintAssignable)
    FOnDebuffApplied OnDebuffApplied;

    UPROPERTY(BlueprintAssignable)
    FOnDebuffRemoved OnDebuffRemoved;

    UPROPERTY(BlueprintAssignable)
    FOnDebuffCountChanged OnDebuffCountChanged;

private:
    int32 iCurrentCnt = 0;
    int32 iMaxCnt = 3;

public:
    FORCEINLINE int32 GetCurrentDebuffCount() const { return iCurrentCnt; }
    FORCEINLINE int32 GetMaxDebuffCount() const { return iMaxCnt; }
    FORCEINLINE void SetCurrentDebuffCount(int32 count) { iCurrentCnt = count; }
    FORCEINLINE void SetMaxDebuffCount(int32 count) { iMaxCnt = count; }



protected:
	virtual void BeginPlay() override;

    // 활성 디버프 맵 (타입 -> {데이터, 남은시간})
    UPROPERTY()
    TMap<EDebuffType, FDebuffData> ActiveDebuffs;

    UPROPERTY()
    TMap<EDebuffType, float> DebuffTimers;

    // 디버프 효과 적용
    void ApplyDebuffEffect(const FDebuffData& DebuffData);
    void RemoveDebuffEffect(EDebuffType DebuffType);

    // 원래 값 저장 (복원용)
    float OriginalMaxWalkSpeed;
    float OriginalDefenseMultiplier;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
