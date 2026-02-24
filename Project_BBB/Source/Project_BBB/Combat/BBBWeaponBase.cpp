// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBWeaponBase.h"

// Sets default values
ABBBWeaponBase::ABBBWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 무기 메시 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	//충돌 비활성화
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponMesh->SetSimulatePhysics(false);

	// 기본값
	FireRate = 0.1f;  // 초당 10발
	bCanAttack = true;
}

void ABBBWeaponBase::Equip(ACharacter* Character)
{
}

void ABBBWeaponBase::Unequip()
{
}

// Called when the game starts or when spawned
void ABBBWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

