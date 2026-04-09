// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyMelee.h"
#include "Character/BBBStatComponent.h"

#include "Components/CapsuleComponent.h"
#include "Combat/BBBDebuffComponent.h"

#include "AI/BBBMeleeAIController.h"

ABBBEnemyMelee::ABBBEnemyMelee()
{
	RangedEnemy = false;
	Damage = 20.f;
	AttackRange = 50.f;

	//HP
	if (StatComponent)
	{
		StatComponent->MaxHP = 20.0f;
		StatComponent->CurrentHP = StatComponent->MaxHP;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT(" %s: No StatComponent"), *GetOwner()->GetName());
	}

	// Mesh
	SetupCharacterMesh();

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
	
	if (StatComponent) {
		StatComponent->CurrentHP = StatComponent->MaxHP;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s BeginPlay : No StatComponent"), *GetOwner()->GetName());
	}
	
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

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/BBB/Actors/Enemy/ABP_MeleeEnemy.ABP_MeleeEnemy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}
