// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBDebuffComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UBBBDebuffComponent::UBBBDebuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OriginalMaxWalkSpeed = 500.0f;
	OriginalDefenseMultiplier = 1.0f;// ...
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
	
}

void UBBBDebuffComponent::ApplyDebuff(const FDebuffData& DebuffData)
{
    // 이미 같은 디버프가 있는 경우
    if (ActiveDebuffs.Contains(DebuffData.DebuffType))
    {
        if (!DebuffData.bStackable)
        {
            // 시간만 갱신
            DebuffTimers[DebuffData.DebuffType] = DebuffData.Duration;
            return;
        }
    }

    // 새 디버프 추가
    ActiveDebuffs.Add(DebuffData.DebuffType, DebuffData);
    DebuffTimers.Add(DebuffData.DebuffType, DebuffData.Duration);

    // 효과 적용
    ApplyDebuffEffect(DebuffData);

    // 델리게이트 브로드캐스트
    OnDebuffApplied.Broadcast(DebuffData.DebuffType, DebuffData.Duration);

    UE_LOG(LogTemp, Warning, TEXT("Debuff Applied: %d for %.1f seconds"),
        (int32)DebuffData.DebuffType, DebuffData.Duration);
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
        Character->GetCharacterMovement()->MaxWalkSpeed =
            OriginalMaxWalkSpeed * (1.0f - DebuffData.Magnitude);
        break;

    case EDebuffType::Weaken:
        // 방어력 감소 (나중에 데미지 계산 시 사용)
        // !!! 나중에 구현 !!!
        break;

    case EDebuffType::Blind:
        // AI 시야 감소 (AI 구현 시)
        // !!! 나중에 구현 !!!
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

    UE_LOG(LogTemp, Warning, TEXT("Debuff Removed: %d"), (int32)DebuffType);
}

bool UBBBDebuffComponent::HasDebuff(EDebuffType DebuffType) const
{
    return ActiveDebuffs.Contains(DebuffType);
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

