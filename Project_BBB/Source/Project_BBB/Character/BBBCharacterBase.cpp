// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BBBCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BBBCharacterControlData.h"
#include "Combat/BBBDebuffComponent.h"
#include "Combat/BBBWeaponBase.h"

// Sets default values
ABBBCharacterBase::ABBBCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Pawn 
	// 회전에 대한 폰의 기본값 설정
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	// 충돌 캡슐 설정
	GetCapsuleComponent()->InitCapsuleSize(7.f, 10.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	// 애니메이션에 맞춰 상대 위치와 회전 지정 필요
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	
	// 숄더
	static ConstructorHelpers::FObjectFinder<UBBBCharacterControlData> ShoulderDataRef(TEXT("/Script/Project_BBB.BBBCharacterControlData'/Game/BBB/CharacterControl/ABC_Shoulder.ABC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	// 쿼터
	static ConstructorHelpers::FObjectFinder<UBBBCharacterControlData> QuaterDataRef(TEXT("/Script/Project_BBB.BBBCharacterControlData'/Game/BBB/CharacterControl/ABC_Quater.ABC_Quater'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	// 1인칭
	static ConstructorHelpers::FObjectFinder<UBBBCharacterControlData> FirstPersonDataRef(TEXT("/Script/Project_BBB.BBBCharacterControlData'/Game/BBB/CharacterControl/ABC_FirstPerson.ABC_FirstPerson'"));
	if (FirstPersonDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::FirstPerson, FirstPersonDataRef.Object);
	}

	// 디버프 컴포넌트 
	DebuffComponent = CreateDefaultSubobject<UBBBDebuffComponent>(TEXT("DebuffComponent"));

	// 무기 초기값
	bIsRangedMode = true;  // 원거리 모드
	WeaponSocketName = TEXT("hand_r");  // 오른손 소켓

	RangedWeapon = nullptr;
	MeleeWeapon = nullptr;
	CurrentWeapon = nullptr;
}

void ABBBCharacterBase::SetCharacterControlData(const UBBBCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUserControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void ABBBCharacterBase::SetupCharacterMesh()
{
	
}


//================================================
// Wepone Section


void ABBBCharacterBase::EquipWeapon(TSubclassOf<class ABBBWeaponBase> WeaponClass, bool bIsRanged)
{
	if (!WeaponClass) return;

	// 무기 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	ABBBWeaponBase* NewWeapon = GetWorld()->SpawnActor<ABBBWeaponBase>(
		WeaponClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (NewWeapon)
	{
		// 손에 부착
		NewWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			WeaponSocketName
		);

		// 해당 슬롯에 저장
		if (bIsRanged)
		{
			RangedWeapon = NewWeapon;
		}
		else
		{
			MeleeWeapon = NewWeapon;
		}

		// 무기 주인
		NewWeapon->SetOwner(this);

		UE_LOG(LogTemp, Warning, TEXT("Weapon equipped: %s"), *NewWeapon->GetName());
	}
}

void ABBBCharacterBase::UnequipWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}

void ABBBCharacterBase::SwitchWeapon()
{
	// 현재 무기 숨기기
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	// 모드 전환
	bIsRangedMode = !bIsRangedMode;

	// 해당 무기 표시
	if (bIsRangedMode)
	{
		EquipRangedWeapon();
	}
	else
	{
		EquipMeleeWeapon();
	}

	UE_LOG(LogTemp, Warning, TEXT("Switched to %s mode"),
		bIsRangedMode ? TEXT("Ranged") : TEXT("Melee"));
}

void ABBBCharacterBase::EquipRangedWeapon()
{
	if (RangedWeapon)
	{
		CurrentWeapon = RangedWeapon;
		CurrentWeapon->SetActorHiddenInGame(false);
	}
}

void ABBBCharacterBase::EquipMeleeWeapon()
{
	if (MeleeWeapon)
	{
		CurrentWeapon = MeleeWeapon;
		CurrentWeapon->SetActorHiddenInGame(false);
	}
}

