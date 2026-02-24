// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BBBCharacterBase.generated.h"

class UBBBDebuffComponent;
class ABBBWeaponBase;

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater,
	FirstPerson
};

UCLASS()
class PROJECT_BBB_API ABBBCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABBBCharacterBase();


	//컨트롤 데이터 설정 함수
	virtual void SetCharacterControlData(const class UBBBCharacterControlData* CharacterControlData);

	// 데이터를 받아오기 위한 컨테이너
	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UBBBCharacterControlData*> CharacterControlManager;


protected:
	// 각 캐릭터 클래스에서 오버라이드할 메시 설정 함수
	virtual void SetupCharacterMesh();

//================================================
// Wepone Section

public:
	// 무기 장착/해제
	void EquipWeapon(TSubclassOf<class ABBBWeaponBase> WeaponClass, bool bIsRanged);
	void UnequipWeapon();

	// 무기 전환
	void SwitchWeapon();

protected:
	// 디버프 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<class UBBBDebuffComponent> DebuffComponent;

	// 무기 슬롯
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<class ABBBWeaponBase> RangedWeapon;  // 원거리

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<class ABBBWeaponBase> MeleeWeapon;   // 근거리

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<class ABBBWeaponBase> CurrentWeapon;  // 현재 장착된 무기

	// 현재 모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsRangedMode;  // true: 원거리, false: 근거리

	// 각 무기 장착 함수
	void EquipRangedWeapon();
	void EquipMeleeWeapon();

	// 무기 소켓 이름
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponSocketName;

};
