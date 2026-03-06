// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBWeaponMelee.h"
#include "Combat/BBBDebuffComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


ABBBWeaponMelee::ABBBWeaponMelee()
{
    WeaponType = EWeaponType::Melee;
    Damage = 50.0f;
    AttackRange = 50.0f; 
}

void ABBBWeaponMelee::Attack()
{
    PerformMeleeAttack();
}

void ABBBWeaponMelee::StopAttack()
{
    UE_LOG(LogTemp, Warning, TEXT("Stop Melee Attack!"));
}

void ABBBWeaponMelee::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABBBWeaponMelee::PerformMeleeAttack()
{
    if (!GetOwner()) return;

    //캐릭터 앞 방향으로 Sphere Trace
    FVector StartLocation = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * AttackRange);

    // Sphere Trace 파라미터
    float SphereRadius = 45.0f;
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
        ECC_Pawn,  // Pawn 채널만
        FCollisionShape::MakeSphere(SphereRadius),
        QueryParams
    );

    // 디버그 드로우 (테스트용)
    //DrawDebugSphere(
    //    GetWorld(),
    //    EndLocation,
    //    SphereRadius,
    //    12,
    //    bHit ? FColor::Red : FColor::Green,
    //    false,
    //    1.0f
    //);

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor || HitActor == GetOwner()) continue;

            // 데미지 적용 (나중에 HealthComponent로 교체)
            UE_LOG(LogTemp, Warning, TEXT("Melee Hit: %s, Damage: %.1f"), *HitActor->GetName(), Damage);
        }
    }
}
