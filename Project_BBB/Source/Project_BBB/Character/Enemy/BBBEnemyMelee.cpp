// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyMelee.h"
#include "Character/BBBStatComponent.h"

#include "Components/CapsuleComponent.h"
#include "Combat/BBBDebuffComponent.h"

#include "AI/BBBMeleeAIController.h"

ABBBEnemyMelee::ABBBEnemyMelee()
{
	// Mesh
	SetupCharacterMesh();

	RangedEnemy = false;
	Damage = 20.f;
	AttackRange = 50.f;

	if (StatComponent)
	{
		StatComponent->MaxHP = 20.0f;
		StatComponent->CurrentHP = StatComponent->MaxHP;
	}

	EnemyInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyInfoWidget"));
	EnemyInfoWidgetComponent->SetupAttachment(GetMesh(), FName("HeadUI")); // 머리 소켓에 붙이기
	EnemyInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 항상 카메라를 바라봄
	EnemyInfoWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));


	// AI 컨트롤러 자동 생성
	AIControllerClass = ABBBMeleeAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABBBEnemyMelee::BeginPlay()
{
	Super::BeginPlay();

	// 디버프 쉴드 초기화
	DebuffComponent->SetCurrentDebuffCount(3);
	DebuffComponent->SetMaxDebuffCount(3);


	EquipMeleeWeapon();

	UpdateEnemyInfoWidget();
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
