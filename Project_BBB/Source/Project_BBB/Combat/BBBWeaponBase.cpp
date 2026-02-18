// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BBBWeaponBase.h"

// Sets default values
ABBBWeaponBase::ABBBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called every frame
void ABBBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

