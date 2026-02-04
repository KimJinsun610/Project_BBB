// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BBBCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
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


};
