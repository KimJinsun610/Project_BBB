// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BBBPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API UBBBPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBBBPlayerAnimInstance();

protected:
	// 이벤트 함수
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;


	//캐릭터 정보
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	// 값
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsGrounded : 1;


//==================================================
// Attack
protected:
	// aim
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimYaw;

	//Shooting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsShooting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* ShootingMontage;

public:
	// Shooting 재생 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayShootingAnimation();

	// Aim 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SetAiming(bool bAiming);
};
