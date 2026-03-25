// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyBase.h"

#include "Character/BBBHealthComponent.h"
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
    if (HPComponent)
    {
        HPComponent->OnHPChanged.AddDynamic(this, &ABBBEnemyBase::OnHPChangedCallback);
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
        Params.CurrentHP = HPComponent->CurrentHP;
        Params.MaxHP = HPComponent->MaxHP;

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
