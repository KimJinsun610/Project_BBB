// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBWeaponRanged.h"
#include "Combat/Projectile/BBBProjectileDebuff.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/BBBCharacterPlayer.h" 
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
    if (!ProjectileClass || !GetOwner()) return;

    ABBBCharacterPlayer* PlayerCharacter = Cast<ABBBCharacterPlayer>(GetOwner());
    if (!PlayerCharacter) return;

    // 총구 위치
    FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);

    // 발사 방향 계산
    FVector LaunchDirection;

    if (PlayerCharacter->bIsAiming)
    {
        // 조준 중: 크로스헤어 방향
        LaunchDirection = PlayerCharacter->GetCrosshairDirection();
        
    }
    else
    {
        // 조준 안 함: 캐릭터 정면
        LaunchDirection = PlayerCharacter->GetActorForwardVector();
    
    }

    // 발사체 생성
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = Cast<APawn>(GetOwner());

    ABBBProjectileDebuff* Projectile = GetWorld()->SpawnActor<ABBBProjectileDebuff>(
        ProjectileClass,
        MuzzleLocation,
        LaunchDirection.Rotation(),
        SpawnParams
    );

    if (Projectile)
    {
        // 디버프 설정
        Projectile->Initialize(DebuffToApply);

        UE_LOG(LogTemp, Warning, TEXT("Projectile fired - Aiming: %d"),
            PlayerCharacter->bIsAiming);
    }
}

void ABBBWeaponRanged::ResetFire()
{
    bCanFire = true;
}
