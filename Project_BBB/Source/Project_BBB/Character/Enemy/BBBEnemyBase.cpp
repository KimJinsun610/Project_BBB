// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyBase.h"

#include "Character/BBBStatComponent.h"
#include "Combat/BBBDebuffComponent.h"

#include "Components/CapsuleComponent.h"


ABBBEnemyBase::ABBBEnemyBase()
{
	// 적 전용 CapsuleConponent
	GetCapsuleComponent()->InitCapsuleSize(4.f, 9.0f);

}

void ABBBEnemyBase::BeginPlay()
{
    Super::BeginPlay();

    // HP가 바뀔 때마다 UI 업데이트
    if (StatComponent)
    {
        StatComponent->OnHPChanged.AddDynamic(this, &ABBBEnemyBase::OnHPChangedCallback);
    }

    // 디버프가 추가/제거될 때마다 UI 업데이트
    if (DebuffComponent)
    {
        DebuffComponent->OnDebuffApplied.AddDynamic(this, &ABBBEnemyBase::OnDebuffAppliedCallback);
        DebuffComponent->OnDebuffRemoved.AddDynamic(this, &ABBBEnemyBase::OnDebuffRemovedCallback);
    }

}

void ABBBEnemyBase::SetupCharacterMesh()
{
}

void ABBBEnemyBase::UpdateEnemyInfoWidget()
{
    UUserWidget* W = EnemyInfoWidgetComponent->GetUserWidgetObject();
    if (!W)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyInfoWidgetComponent is Null"));
        return;
    }

    UFunction* Func = W->FindFunction(FName("SetEnemyInfo"));
    if (Func)
    {
        struct FParams
        {
            int CurrentHP;
            int MaxHP;
        };
        FParams Params;
        Params.CurrentHP = StatComponent->CurrentHP;
        Params.MaxHP = StatComponent->MaxHP;

        W->ProcessEvent(Func, &Params);

    }
}


void ABBBEnemyBase::OnHPChangedCallback(float CurrentHP, float MaxHP, AActor* DamageCauser)
{
    UpdateEnemyInfoWidget();
}

void ABBBEnemyBase::OnDebuffAppliedCallback(EDebuffType DebuffType, float Duration)
{
    UpdateEnemyInfoWidget();
}

void ABBBEnemyBase::OnDebuffRemovedCallback(EDebuffType DebuffType)
{
    UpdateEnemyInfoWidget();
}

float ABBBEnemyBase::GetAIPatrolRadius()
{
    return 800.0f;
}

float ABBBEnemyBase::GetAIDetectRange()
{
    return 400.0f;
}

float ABBBEnemyBase::GetAIAttackRange()
{
    if (!StatComponent) return 0.0f;
    return StatComponent->GetAttackRadius()*2;
}

float ABBBEnemyBase::GetAITurnSpeed()
{
    return 0.0f;
}

void ABBBEnemyBase::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
    OnAttackFinished = InOnAttackFinished;
}

void ABBBEnemyBase::AttackByAI()
{
}
