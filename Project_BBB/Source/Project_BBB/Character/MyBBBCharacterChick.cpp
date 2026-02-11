// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyBBBCharacterChick.h"
#include "Components/CapsuleComponent.h"

AMyBBBCharacterChick::AMyBBBCharacterChick()
{
	// 적 전용 CapsuleConponent
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// AI 컨트롤러 자동 생성 (나중에 AI 추가 시)
	AIControllerClass = nullptr; // 나중에 AI Controller 클래스 지정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Mesh
	SetupCharacterMesh();
}

void AMyBBBCharacterChick::SetupCharacterMesh()
{

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BBB/Models/Enemys/Chicken/Chicken.Chicken'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/BBB/Models/Enemys/Chicken/ABP_Chick.ABP_Chick_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}
