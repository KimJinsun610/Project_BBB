// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyRanged.h"
#include "Components/CapsuleComponent.h"
#include "Character/BBBHealthComponent.h"


ABBBEnemyRanged::ABBBEnemyRanged()
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
		HPComponent->MaxHP = 20.0f;
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
