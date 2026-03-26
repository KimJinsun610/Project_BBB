// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyRanged.h"
#include "Components/CapsuleComponent.h"
#include "Character/BBBStatComponent.h"


ABBBEnemyRanged::ABBBEnemyRanged()
{

	// Mesh
	SetupCharacterMesh();

	//HP
	if (StatComponent)
	{
		StatComponent->MaxHP = 100.0f;
		StatComponent->CurrentHP = StatComponent->MaxHP;
	}


	EnemyInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyInfoWidget"));
	EnemyInfoWidgetComponent->SetupAttachment(GetMesh(), FName("HeadUI")); // 머리 소켓에 붙이기
	EnemyInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 항상 카메라를 바라봄
	EnemyInfoWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));

	// AI 컨트롤러 자동 생성 (나중에 AI 추가 시)
	AIControllerClass = nullptr; // 나중에 AI Controller 클래스 지정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ABBBEnemyRanged::BeginPlay()
{
	Super::BeginPlay();
	UpdateEnemyInfoWidget();
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
