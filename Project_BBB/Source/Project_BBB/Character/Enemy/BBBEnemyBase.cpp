// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyBase.h"

#include "Character/BBBStatComponent.h"
#include "Combat/BBBDebuffComponent.h"
#include "Combat/BBBWeaponRanged.h"
#include "Combat/BBBWeaponMelee.h"
#include "Physics/BBBCollision.h"
#include "Item/BBBItemBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ABBBEnemyBase::ABBBEnemyBase()
{
	// 적 전용 CapsuleConponent
	GetCapsuleComponent()->InitCapsuleSize(4.f, 9.0f);

    Damage = 20.f;
    AttackRange = 50.f;

}

void ABBBEnemyBase::BeginPlay()
{
    Super::BeginPlay();


    // Stat관련 업데이트
    if (StatComponent)
    {
        StatComponent->OnHPChanged.AddDynamic(this, &ABBBEnemyBase::OnHPChangedCallback);
        StatComponent->OnDeath.AddDynamic(this, &ABBBEnemyBase::OnEnemyDeath);
    }

    // 디버프가 추가/제거될 때마다 UI 업데이트
    if (DebuffComponent)
    {
        DebuffComponent->OnDebuffApplied.AddDynamic(this, &ABBBEnemyBase::OnDebuffAppliedCallback);
        DebuffComponent->OnDebuffRemoved.AddDynamic(this, &ABBBEnemyBase::OnDebuffRemovedCallback);
        DebuffComponent->OnDebuffCountChanged.AddDynamic(this, &ABBBEnemyBase::OnDebuffChangeCallback);
    }

    // 한 프레임 뒤로 미룸
    GetWorldTimerManager().SetTimerForNextTick([this]()
    {
        UpdateEnemyInfoWidget();
        UpdateEnemyDebuff();
    });

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

void ABBBEnemyBase::UpdateEnemyDebuff()
{
    UUserWidget* W = EnemyInfoWidgetComponent->GetUserWidgetObject();
    if (!W)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyInfoWidgetComponent is Null"));
        return;
    }

    UFunction* Func2 = W->FindFunction(FName("SetDebuffCnt"));
    if (Func2)
    {
        struct FParams
        {
            int Count;
        };
        FParams Params;
        Params.Count = DebuffComponent->GetCurrentDebuffCount();

        W->ProcessEvent(Func2, &Params);

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
    UpdateEnemyDebuff();
}

void ABBBEnemyBase::OnDebuffChangeCallback(EDebuffType DebuffType)
{
    UpdateEnemyDebuff();
}


//Attack

void ABBBEnemyBase::PerformAttack()
{
    if (DebuffComponent->HasAnyDebuff()) return;

    if (RangedEnemy) {


    }
    else {
        PerformMeleeAttack();
        MeleeAttackBegin();
    }
}

void ABBBEnemyBase::PerformMeleeAttack() {

    if (!GetOwner()) return;

    UE_LOG(LogTemp, Warning, TEXT("Melee Enemy Hit"));

    //캐릭터 앞 방향으로 Sphere Trace
    FVector StartLocation = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * AttackRange);

    // Sphere Trace 파라미터
    float SphereRadius = 150.0f;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.AddIgnoredActor(this);

    // Multi Sphere Trace (여러 적 동시 공격 가능)
    TArray<FHitResult> HitResults;
    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        StartLocation,
        EndLocation,
        FQuat::Identity,
        CCHANNEL_BBBACTION,  
        FCollisionShape::MakeSphere(SphereRadius),
        QueryParams
    );

   DrawDebugSphere(
      GetWorld(),
      EndLocation,
      SphereRadius,
      12,
      bHit ? FColor::Red : FColor::Green,
      false,
      1.0f
   );


    if (bHit)
    {
        TSet<AActor*> HitActors;

        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor || HitActor == GetOwner()) continue;

            if (HitActors.Contains(HitActor)) continue;
            HitActors.Add(HitActor);

            UBBBStatComponent* HealthComp = HitActor->FindComponentByClass<UBBBStatComponent>();
            if (HealthComp)
            {
                HealthComp->TakeDamage(Damage, GetOwner());

                UE_LOG(LogTemp, Warning, TEXT("Melee Hit: %s, Damage: %.1f"), *HitActor->GetName(), Damage);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Hit %s but no HealthComponent"), *HitActor->GetName());
            }
        }
    }
}

void ABBBEnemyBase::MeleeAttackBegin()
{
    if (!MeleeAttackMontage)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing data! - Enemy Melee Attack Mongtage Is Missing"));
        return;
    }
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("AnimInstance is NULL!"));
        return;
    }


    if (AnimInstance->Montage_IsPlaying(MeleeAttackMontage))
    {
        AnimInstance->Montage_Stop(0.1f, MeleeAttackMontage);
    }

    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

    //Animation Setting
    const float AttackSpeedRate = 1.0f;

    AnimInstance->Montage_Play(MeleeAttackMontage, AttackSpeedRate);

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &ABBBEnemyBase::MeleeAttackEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);

}

void ABBBEnemyBase::MeleeAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    OnAttackFinished.ExecuteIfBound();
}





// AI
float ABBBEnemyBase::GetAIPatrolRadius()
{
    return 800.0f;
}

float ABBBEnemyBase::GetAIDetectRange()
{
    return 800.0f;
}

float ABBBEnemyBase::GetAIAttackRange()
{
    if (!StatComponent) {
        UE_LOG(LogTemp, Error, TEXT("No StatsComponent"));
        return 0.0f;
    }

    return StatComponent->GetAttackRadius()*3;
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
    PerformAttack();
}

// Item
void ABBBEnemyBase::DropItems()
{
    for (const FItemDropData& DropData : DropTable)
    {
        if (!DropData.ItemClass) continue;

        // 수량 결정
        int32 Amount = 1;

        // 아이템 스폰
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = GetActorLocation() + FVector(FMath::RandRange(-50.f, 50.f), FMath::RandRange(-50.f, 50.f), 50.f);

        ABBBItemBase* Item = GetWorld()->SpawnActor<ABBBItemBase>(DropData.ItemClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

        if (Item)
        {
            Item->SetAmount(Amount);
        }
    }
}

void ABBBEnemyBase::OnEnemyDeath(AActor* Killed, AActor* Killer)
{
    DropItems();
}
