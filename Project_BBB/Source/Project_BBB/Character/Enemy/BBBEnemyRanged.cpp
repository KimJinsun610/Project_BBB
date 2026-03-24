// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyRanged.h"
#include "Components/CapsuleComponent.h"
#include "Character/BBBHealthComponent.h"


ABBBEnemyRanged::ABBBEnemyRanged()
{

	// Mesh
	SetupCharacterMesh();

	//HP
	if (HPComponent)
	{
		HPComponent->MaxHP = 20.0f;
		HPComponent->CurrentHP = HPComponent->MaxHP;
	}
}

void ABBBEnemyRanged::SetupCharacterMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BBB/Models/Enemys/MonsterForSurvivalGame/Mesh/PBR/Swarm08_SK.Swarm08_SK'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/BBB/Characters/Enemy/ABP_RangedEnemy.ABP_RangedEnemy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}
