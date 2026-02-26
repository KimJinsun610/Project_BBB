// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBWeaponRanged.h"

ABBBWeaponRanged::ABBBWeaponRanged()
{
		///!!!
		//충돌 비활성화
}

void ABBBWeaponRanged::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Ranged Attack"));
}

void ABBBWeaponRanged::StopAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Ranged Attack"));
}

void ABBBWeaponRanged::FireProjectile()
{
}
