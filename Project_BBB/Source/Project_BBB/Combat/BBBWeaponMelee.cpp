// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBWeaponMelee.h"


ABBBWeaponMelee::ABBBWeaponMelee()
{
    // !!!! 나중에 구현 !!!!

}

void ABBBWeaponMelee::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("Melee Attack!"));
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
}
