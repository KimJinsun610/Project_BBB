// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BBBCharacterBase.h"
#include "InputActionValue.h"
#include "BBBCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBCharacterPlayer: public ABBBCharacterBase
{
	GENERATED_BODY()

public:
	ABBBCharacterPlayer();

protected:
	virtual void BeginPlay() override;

	virtual void SetupCharacterMesh() override;

//================================================
protected: // Camera Section
	
	// 카메라 스플링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	// Carmera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	// 카메라 뷰 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	bool bIsFirstPersonView;  // true: 1인칭, false: 3인칭

	// 카메라 전환 함수
	void SwitchToFirstPersonView();
	void SwitchToThirdPersonView();

//================================================
// Character Control Section
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const class UBBBCharacterControlData* CharacterControlData) override;



//================================================
// Input Section

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	// 숄더뷰
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	// 쿼터 뷰
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	// 1인칭
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstPersonMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FirstPersonLookAction;


	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void FirstPersonMove(const FInputActionValue& Value);
	void FirstPersonLook(const FInputActionValue& Value);

	// 카메라 변경
	ECharacterControlType CurrentCharacterControlType;

	//Carmera Arm Control
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraZoomAction;

//================================================
// Combat Section

protected:
	// 무기 전환키 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SwitchWeaponAction;

	// 공격 (마우스 좌클릭)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	// 입력 함수
	void SwitchWeaponMode(const FInputActionValue& Value);
	void PerformAttack(const FInputActionValue& Value);
	void StopAttack(const FInputActionValue& Value);

protected:
	// 무기 블루프린트 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ABBBWeaponBase> RangedWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ABBBWeaponBase> MeleeWeaponClass;

protected: //근거리 공격

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> MeleeAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBBBComboActionAsset> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

protected: // 원거리 공격
	//Shooting Montage 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* ShootingMontage;

	// 조준
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputAction> AimAction;

	// Aim 입력 함수
	void StartAim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);

	// 애니메이션 재생 함수
	void PlayShootingAnimation();
	

public:
	// Aim 상태
	bool bIsAiming;

//================================================
// UI Section
protected:

	// 크로스 헤어
	UPROPERTY(BlueprintReadOnly, Category = UI)
	bool bShowCrosshair;

public:
	// 화면 중앙 계산용
	FVector GetCrosshairWorldLocation() const;
	FVector GetCrosshairDirection() const;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdatePlayerWidget();

private:
	// HP 변경 콜백
	UFUNCTION()
	void OnHPChangedCallback(float CurrentHP, float MaxHP, AActor* DamageCauser);

//================================================
// Gold Section
public:
	UFUNCTION(BlueprintCallable, Category = "Gold")
	void AddGold(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Gold")
	int32 GetGold() const { return Gold; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gold", Meta = (AllowPrivateAccess = "true"))
	int32 Gold = 0;
};
