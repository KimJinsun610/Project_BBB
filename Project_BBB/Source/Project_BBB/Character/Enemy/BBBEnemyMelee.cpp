// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyMelee.h"
#include "Components/CapsuleComponent.h"
#include "Character/BBBHealthComponent.h"

ABBBEnemyMelee::ABBBEnemyMelee()
{
	// 적 전용 CapsuleConponent
	GetCapsuleComponent()->InitCapsuleSize(4.f, 9.0f);

	// AI 컨트롤러 자동 생성 (나중에 AI 추가 시)
	AIControllerClass = nullptr; // 나중에 AI Controller 클래스 지정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Mesh
	SetupCharacterMesh();

	if (HPComponent)
	{
		HPComponent->MaxHP = 50.0f;
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
