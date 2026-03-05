// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBWeaponRanged.h"
#include "Combat/Projectile/BBBProjectileDebuff.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ABBBWeaponRanged::ABBBWeaponRanged()
{
    WeaponType = EWeaponType::Ranged;
    MuzzleSocketName = TEXT("Muzzle");  // 총구 소켓 이름
    FireDelay = 0.2f;  // 0.2초 간격
    bCanFire = true;

    // 기본 디버프 설정 (Stun 3초)
    DebuffToApply.DebuffType = EDebuffType::Stun;
    DebuffToApply.Duration = 3.0f;
    DebuffToApply.Magnitude = 1.0f;
    DebuffToApply.bStackable = false;
}

void ABBBWeaponRanged::Attack()
{
    if (bCanFire)
    {
        FireProjectile();

        // 연사 제한
        bCanFire = false;
        GetWorld()->GetTimerManager().SetTimer(
            FireTimerHandle,
            this,
            &ABBBWeaponRanged::ResetFire,
            FireDelay,
            false
        );
    }
}

void ABBBWeaponRanged::StopAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Ranged Attack"));
}

void ABBBWeaponRanged::FireProjectile()
{
    if (!ProjectileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ProjectileClass is NULL!"));
        return;
    }

    // 총구 위치 가져오기
    FVector MuzzleLocation;
    FRotator MuzzleRotation;

    // Muzzle 소켓이 있으면 소켓 위치 사용
    if (WeaponMesh && WeaponMesh->DoesSocketExist(MuzzleSocketName))
    {
        MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
        MuzzleRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
    else
    {
        // 소켓 없으면 무기 위치 사용
        MuzzleLocation = GetActorLocation();
        MuzzleRotation = GetActorRotation();

        // 캐릭터 정면 방향으로 발사
        if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
        {
            MuzzleRotation = OwnerChar->GetControlRotation();
        }
    }

    // 발사체 생성
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = Cast<APawn>(GetOwner());
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ABBBProjectileDebuff* Projectile = GetWorld()->SpawnActor<ABBBProjectileDebuff>(
        ProjectileClass,
        MuzzleLocation,
        MuzzleRotation,
        SpawnParams
    );

    if (Projectile)
    {
        // 디버프 정보 전달
        Projectile->Initialize(DebuffToApply);

        UE_LOG(LogTemp, Warning, TEXT("Projectile Fired! Debuff Type: %d"), (int32)DebuffToApply.DebuffType);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile!"));
    }
}

void ABBBWeaponRanged::ResetFire()
{
    bCanFire = true;
}
