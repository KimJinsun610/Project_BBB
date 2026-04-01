// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBDebuffComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UBBBDebuffComponent::UBBBDebuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OriginalMaxWalkSpeed = 500.0f;
	OriginalDefenseMultiplier = 1.0f;
}

// Called when the game starts
void UBBBDebuffComponent::BeginPlay()
{
	Super::BeginPlay();

	// 원래 값 저장
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		OriginalMaxWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	}

    iCurrentCnt = iMaxCnt;
	
}

void UBBBDebuffComponent::ApplyDebuff(const FDebuffData& DebuffData)
{
    EDebuffType Type = DebuffData.DebuffType;

    // 카운트 감소
    iCurrentCnt = FMath::Max(0, iCurrentCnt - 1);

    // 카운트 변경 브로드캐스트
    OnDebuffCountChanged.Broadcast(Type);

    // 카운트가 0이 되면 디버프 적용
    if (iCurrentCnt <= 0)
    {
        if (ActiveDebuffs.Contains(Type))
        {
            if (!DebuffData.bStackable)
            {
                DebuffTimers[Type] = DebuffData.Duration;
                return;
            }
        }

        ActiveDebuffs.Add(Type, DebuffData);
        DebuffTimers.Add(Type, DebuffData.Duration);
        ApplyDebuffEffect(DebuffData);
        OnDebuffApplied.Broadcast(Type, DebuffData.Duration);

        UE_LOG(LogTemp, Warning, TEXT("Debuff Applied: %d"), (int32)Type);
    }
    

}

void UBBBDebuffComponent::ApplyDebuffEffect(const FDebuffData& DebuffData)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    switch (DebuffData.DebuffType)
    {
    case EDebuffType::Stun:
        // 이동 불가
        Character->GetCharacterMovement()->DisableMovement();
        break;

    case EDebuffType::Slow:
        // 이동속도 감소
        Character->GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed * (1.0f - DebuffData.Magnitude);
        break;

    case EDebuffType::Weaken:
        // 방어력 감소 (나중에 데미지 계산 시 사용)
        // !!!! 나중에 구현 !!!!
        break;

    case EDebuffType::Blind:
        // AI 시야 감소
        // !!!! 나중에 구현 !!!!
        break;

    default:
        break;
    }
}

void UBBBDebuffComponent::RemoveDebuffEffect(EDebuffType DebuffType)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    switch (DebuffType)
    {
    case EDebuffType::Stun:
        // 이동 복구
        Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
        break;

    case EDebuffType::Slow:
        // 이동속도 복구
        Character->GetCharacterMovement()->MaxWalkSpeed = OriginalMaxWalkSpeed;
        break;

    default:
        break;
    }
}

void UBBBDebuffComponent::RemoveDebuff(EDebuffType DebuffType)
{
    if (!ActiveDebuffs.Contains(DebuffType)) return;

    // 효과 제거
    RemoveDebuffEffect(DebuffType);

    // 데이터 제거
    ActiveDebuffs.Remove(DebuffType);
    DebuffTimers.Remove(DebuffType);

    // 델리게이트
    OnDebuffRemoved.Broadcast(DebuffType);

    iCurrentCnt = iMaxCnt;
    OnDebuffCountChanged.Broadcast(DebuffType);
}

bool UBBBDebuffComponent::HasDebuff(EDebuffType DebuffType) const
{
    return ActiveDebuffs.Contains(DebuffType);
}

bool UBBBDebuffComponent::HasAnyDebuff() const
{
    return ActiveDebuffs.Num() > 0;
}

TArray<FDebuffData> UBBBDebuffComponent::GetActiveDebuffs() const
{
    TArray<FDebuffData> Result;
    ActiveDebuffs.GenerateValueArray(Result);
    return Result;
}


void UBBBDebuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 디버프 타이머 감소
    TArray<EDebuffType> ExpiredDebuffs;

    for (auto& Pair : DebuffTimers)
    {
        Pair.Value -= DeltaTime;

        if (Pair.Value <= 0.0f)
        {
            ExpiredDebuffs.Add(Pair.Key);
        }
    }

    // 만료된 디버프 제거
    for (EDebuffType DebuffType : ExpiredDebuffs)
    {
        RemoveDebuff(DebuffType);
    }
}

