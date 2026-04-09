// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyRanged.h"

#include "AI/BBBRangedAIController.h"
#include "Combat/BBBDebuffComponent.h"
#include "Combat/BBBWeaponRanged.h"

#include "Components/CapsuleComponent.h"
#include "Character/BBBStatComponent.h"

ABBBEnemyRanged::ABBBEnemyRanged()
{

	//HP
	if (StatComponent)
	{
		StatComponent->MaxHP = 20.0f;
		StatComponent->CurrentHP = StatComponent->MaxHP;
	}

	// Mesh
	SetupCharacterMesh();

	EnemyInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyInfoWidget"));
	EnemyInfoWidgetComponent->SetupAttachment(GetMesh(), FName("HeadUI")); // 머리 소켓에 붙이기
	EnemyInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 항상 카메라를 바라봄
	EnemyInfoWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));

	// AI 컨트롤러 생성 
	AIControllerClass = ABBBRangedAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	RangedEnemy = true;

}

void ABBBEnemyRanged::BeginPlay()
{
	Super::BeginPlay();
	if (StatComponent) {
		StatComponent->CurrentHP = StatComponent->MaxHP;
	}


	UpdateEnemyInfoWidget();
}

void ABBBEnemyRanged::SetupCharacterMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BBB/Models/Enemys/MonsterForSurvivalGame/Mesh/PBR/Swarm08_SK.Swarm08_SK'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/BBB/Actors/Enemy/ABP_RangedEnemy.ABP_RangedEnemy_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

float ABBBEnemyRanged::GetAIAttackRange()
{
	return 800.0f;
}
