// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Projectile/BBBProjectileDebuff.h"
#include "Combat/BBBDebuffComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" 

ABBBProjectileDebuff::ABBBProjectileDebuff()
    :Super()
{
    Speed = 3000.0f; // 발사체 속도 변경
}

void ABBBProjectileDebuff::Initialize(const FDebuffData& InDebuffData)
{
    DebuffData = InDebuffData;
    SetProjectileColor();
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = GetActorForwardVector() * Speed;
        UE_LOG(LogTemp, Warning, TEXT("Initialize - Setting Velocity"));
    }
}


void ABBBProjectileDebuff::OnProjectileHit(AActor* HitActor, const FHitResult& Hit)
{

    // 디버프 컴포넌트 찾기
    UBBBDebuffComponent* DebuffComp = HitActor->FindComponentByClass<UBBBDebuffComponent>();

    if (DebuffComp)
    {        
        // 디버프 적용
        DebuffComp->ApplyDebuff(DebuffData);


        UE_LOG(LogTemp, Warning, TEXT("Debuff projectile hit %s! Applied: %d"), *HitActor->GetName(), (int32)DebuffData.DebuffType);
    } 
}

void ABBBProjectileDebuff::SetProjectileColor()
{
    if (!MeshComponent) return;

    UMaterialInstanceDynamic* DynMaterial = MeshComponent->CreateDynamicMaterialInstance(0);
    if (!DynMaterial) return;

    // 디버프 타입별 색상
    FLinearColor Color;
    switch (DebuffData.DebuffType)
    {
    case EDebuffType::Stun:
        Color = FLinearColor::Yellow;
        break;
    case EDebuffType::Slow:
        Color = FLinearColor::Blue;
        break;
    case EDebuffType::Weaken:
        Color = FLinearColor(1.0f, 0.5f, 0.0f);  // 주황색
        break;
    default:
        Color = FLinearColor::White;
        break;
    }

    DynMaterial->SetVectorParameterValue(TEXT("Color"), Color);
}
