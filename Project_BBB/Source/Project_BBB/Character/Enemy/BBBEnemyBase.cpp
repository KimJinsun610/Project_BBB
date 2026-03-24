// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/BBBEnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "Character/BBBHealthComponent.h"


ABBBEnemyBase::ABBBEnemyBase()
{
	// 적 전용 CapsuleConponent
	GetCapsuleComponent()->InitCapsuleSize(4.f, 9.0f);

	// AI 컨트롤러 자동 생성 (나중에 AI 추가 시)
	AIControllerClass = nullptr; // 나중에 AI Controller 클래스 지정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ABBBEnemyBase::SetupCharacterMesh()
{
}
