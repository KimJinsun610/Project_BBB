// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyMelee.h"
#include "Components/CapsuleComponent.h"
#include "Character/BBBHealthComponent.h"

ABBBEnemyMelee::ABBBEnemyMelee()
{

	// Mesh
	SetupCharacterMesh();

	if (HPComponent)
	{
		HPComponent->MaxHP = 20.0f;
		HPComponent->CurrentHP = HPComponent->MaxHP;
	}

}

void ABBBEnemyMelee::SetupCharacterMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BBB/Models/Enemys/MonsterForSurvivalGame/Mesh/PBR/TurtleShell_SK.TurtleShell_SK'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/BBB/Characters/Enemy/ABP_MeleeEnemy.ABP_MeleeEnemy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}
