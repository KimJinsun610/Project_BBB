// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BBBCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Combat/BBBWeaponRanged.h"
#include "Combat/BBBWeaponMelee.h"
#include "Combat/Projectile/BBBComboActionAsset.h"

#include "BBBCharacterControlData.h"
#include "Character/BBBStatComponent.h"
#include "Player/BBBPlayerAnimInstance.h"
#include "Player/BBBPlayerController.h"


#include "Animation/AnimMontage.h"
#include "Physics/BBBCollision.h"



ABBBCharacterPlayer::ABBBCharacterPlayer()
{
	//================================================
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// 3인칭
	CameraBoom->TargetArmLength = 450.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bIsFirstPersonView = false;

	//================================================
	// setup
	GetCapsuleComponent()->InitCapsuleSize(20.f, 50.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_BBPLAYERCAPSULE);


	SetupCharacterMesh();

	// HP
	if (StatComponent)
	{
		StatComponent->MaxHP = 100.0f;
	}

	//================================================
	// Input

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	// 숄더 뷰
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	// 쿼터 뷰
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	//1인칭
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFirstPersonMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_FirstPersonMove.IA_FirstPersonMove'"));
	if (nullptr != InputActionFirstPersonMoveRef.Object)
	{
		FirstPersonMoveAction = InputActionFirstPersonMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFirstPersonLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_FirstPersonLook.IA_FirstPersonLook'"));
	if (nullptr != InputActionFirstPersonLookRef.Object)
	{
		FirstPersonLookAction = InputActionFirstPersonLookRef.Object;
	}

	// 현재 시점
	CurrentCharacterControlType = ECharacterControlType::Shoulder;

	//================================================
	// Attack
	
	bIsAiming = false;
	bShowCrosshair = false;

	//무기 전환
	static ConstructorHelpers::FObjectFinder<UInputAction> InputSwitchWeaponActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_SwitchWeapon.IA_SwitchWeapon'"));
	if (InputSwitchWeaponActionRef.Object)
	{
		SwitchWeaponAction = InputSwitchWeaponActionRef.Object;
	}
	// 공격
	static ConstructorHelpers::FObjectFinder<UInputAction> InputAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_Attack.IA_Attack'"));
	if (InputAttackActionRef.Object)
	{
		AttackAction = InputAttackActionRef.Object;
	}

	// 조준
	static ConstructorHelpers::FObjectFinder<UInputAction> InputAimActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/BBB/Input/Actions/IA_Aim.IA_Aim'"));
	if (InputAimActionRef.Object)
	{
		AimAction = InputAimActionRef.Object;
	}

	// 원거리 
	static ConstructorHelpers::FClassFinder<ABBBWeaponBase> RangedWeaponClassRef( TEXT("/Game/BBB/Characters/Wepones/BP_Gun.BP_Gun_C"));
	if (RangedWeaponClassRef.Class)
	{
		RangedWeaponClass = RangedWeaponClassRef.Class;
	}

	//근거리
	static ConstructorHelpers::FClassFinder<ABBBWeaponBase> MeleeWeaponClassRef(TEXT("/Game/BBB/Characters/Wepones/BP_Sword.BP_Sword_C"));
	if (MeleeWeaponClassRef.Class)
	{

		MeleeWeaponClass = MeleeWeaponClassRef.Class;
	}

}

void ABBBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurrentCharacterControlType);

	// HP가 바뀔 때마다 UI 업데이트
	if (StatComponent)
	{
		StatComponent->OnHPChanged.AddDynamic(this, &ABBBCharacterPlayer::OnHPChangedCallback);
	}

	// 무기 생성
	
	if (RangedWeaponClass)
	{
		EquipWeapon(RangedWeaponClass, true);
	}
	if (MeleeWeaponClass)
	{
		EquipWeapon(MeleeWeaponClass, false);
	}

	// 원거리 모드로 시작
	EquipRangedWeapon();
	
	ABBBPlayerController* PC = Cast<ABBBPlayerController>(GetController());
	if (PC)
	{
		// HP 20 == 1 Heart
		int MaxHP = (StatComponent->MaxHP / 20);
		int CurrentHP = (StatComponent->CurrentHP / 20);

		PC->SetHP(MaxHP, CurrentHP);
	}
}

void ABBBCharacterPlayer::SetupCharacterMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BBB/Models/Player2/Player2_model.Player2_model'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/BBB/Characters/Player/ABP_Player2.ABP_Player2_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

}

void ABBBCharacterPlayer::SwitchToFirstPersonView()
{
	bIsFirstPersonView = true;

	CameraBoom->TargetArmLength = 0.0f;
	CameraBoom->SocketOffset = FVector(10.0f, 0.0f, 70.0f);  // 눈높이

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	UE_LOG(LogTemp, Warning, TEXT("Switched to First Person View"));
}

void ABBBCharacterPlayer::SwitchToThirdPersonView()
{
	bIsFirstPersonView = false;

	// 3인칭: 솔더뷰
	CameraBoom->TargetArmLength = 450.0f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	UE_LOG(LogTemp, Warning, TEXT("Switched to Third Person View"));
}

void ABBBCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::FirstPerson);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::FirstPerson)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void ABBBCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UBBBCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ABBBCharacterPlayer::SetCharacterControlData(const UBBBCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->SocketOffset = CharacterControlData->SocketOffset;
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ABBBCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// 이동
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ABBBCharacterPlayer::ChangeCharacterControl);
	
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ABBBCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ABBBCharacterPlayer::ShoulderLook);
	
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ABBBCharacterPlayer::QuaterMove);

	EnhancedInputComponent->BindAction(FirstPersonMoveAction, ETriggerEvent::Triggered, this, &ABBBCharacterPlayer::FirstPersonMove);
	EnhancedInputComponent->BindAction(FirstPersonLookAction, ETriggerEvent::Triggered, this, &ABBBCharacterPlayer::FirstPersonLook);


	// 무기
	if (SwitchWeaponAction)
	{
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &ABBBCharacterPlayer::SwitchWeaponMode);
	}

	// 공격
	if (AttackAction)
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ABBBCharacterPlayer::PerformAttack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ABBBCharacterPlayer::StopAttack);
	}

	// 조준
	if (AimAction)
	{
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ABBBCharacterPlayer::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ABBBCharacterPlayer::StopAim);
	}
}



void ABBBCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ABBBCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
	
}

void ABBBCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);

}

void ABBBCharacterPlayer::FirstPersonMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 컨트롤러의 현재 회전값 가져오기
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// 전방/우측 방향 벡터 계산
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 이동 입력 적용
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ABBBCharacterPlayer::FirstPersonLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 마우스 입력으로 시점 회전
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ABBBCharacterPlayer::SwitchWeaponMode(const FInputActionValue& Value)
{
	if (!bIsAiming)
	{
		SwitchWeapon();
		ABBBPlayerController* PC = Cast<ABBBPlayerController>(GetController());
		if (PC)
		{
			PC->ShowWeaponInfo(bIsRangedMode);
		}
	}
}

void ABBBCharacterPlayer::PerformAttack(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (CurrentWeapon)
	{
		if (bIsRangedMode && bIsAiming)
		{
			if (CurrentWeapon->bCanFire)PlayShootingAnimation();
			CurrentWeapon->Attack();
		}
		// 근거리 모드는 바로 공격
		else if (!bIsRangedMode)
		{
			ProcessComboCommand();
			CurrentWeapon->Attack();

		}
	}

}

void ABBBCharacterPlayer::StopAttack(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopAttack();
	}
}

void ABBBCharacterPlayer::ProcessComboCommand()
{
	if (!ComboActionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComboActionData is NULL - Skipping combo"));
		return;
	}
	if (!MeleeAttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttackMontage is NULL - Skipping combo"));
		return;
	}


	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void ABBBCharacterPlayer::ComboActionBegin()
{
	if (!ComboActionData || !MeleeAttackMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing data!"));
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is NULL!"));
		return;
	}


	if (AnimInstance->Montage_IsPlaying(MeleeAttackMontage))
	{
		AnimInstance->Montage_Stop(0.1f, MeleeAttackMontage);
	}

	CurrentCombo = 1;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//Animation Setting
	const float AttackSpeedRate = 1.0f;

	AnimInstance->Montage_Play(MeleeAttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABBBCharacterPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, MeleeAttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ABBBCharacterPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ABBBCharacterPlayer::SetComboCheckTimer()
{
	if (!ComboActionData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComboActionData is NULL in SetComboCheckTimer"));
		return;
	}


	int32 ComboIndex = CurrentCombo - 1;

	if (!ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid ComboIndex: %d, Array Size: %d"),
			ComboIndex, ComboActionData->EffectiveFrameCount.Num());
		return;
	}

	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ABBBCharacterPlayer::ComboCheck, ComboEffectiveTime, false);
	}
}

void ABBBCharacterPlayer::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, MeleeAttackMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void ABBBCharacterPlayer::StartAim(const FInputActionValue& Value)
{

	if (!bIsRangedMode)
	{
		return;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}


	bIsAiming = true;
	bShowCrosshair = true;

	// 시점 변경
	SwitchToFirstPersonView();

	// 이동 비활성화
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 크로스헤어 활성화
	ABBBPlayerController* PC = Cast<ABBBPlayerController>(GetController());
	if (PC)
	{
		PC->ShowCrosshair(bShowCrosshair);
	}
	
	
	// AnimInstance에 전달
	UBBBPlayerAnimInstance* AnimInstance = Cast<UBBBPlayerAnimInstance>(
		GetMesh()->GetAnimInstance()
	);

	if (AnimInstance)
	{
		AnimInstance->SetAiming(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Start Aiming - First Person View"));
	
}

void ABBBCharacterPlayer::StopAim(const FInputActionValue& Value)
{
	if (!bIsAiming)
	{
		return;
	}


	bIsAiming = false;
	bShowCrosshair = false;

	// 시점 변경
	SwitchToThirdPersonView();

	// 움직임 활성화
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	//크로스헤어 비활성화
	ABBBPlayerController* PC = Cast<ABBBPlayerController>(GetController());
	if (PC)
	{
		PC->ShowCrosshair(bShowCrosshair);
	}


	UBBBPlayerAnimInstance* AnimInstance = Cast<UBBBPlayerAnimInstance>(
		GetMesh()->GetAnimInstance()
	);

	if (AnimInstance)
	{
		AnimInstance->SetAiming(false);
	}

	UE_LOG(LogTemp, Warning, TEXT("Stop Aiming - Third Person View"));
}

void ABBBCharacterPlayer::PlayShootingAnimation()
{
	UBBBPlayerAnimInstance* AnimInstance = Cast<UBBBPlayerAnimInstance>(
		GetMesh()->GetAnimInstance()
	);

	if (AnimInstance)
	{
		AnimInstance->PlayShootingAnimation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance is not UBBBPlayerAnimInstance!"));
	}
}

FVector ABBBCharacterPlayer::GetCrosshairWorldLocation() const
{
	if (!FollowCamera)
	{
		return FVector::ZeroVector;
	}

	// 크로스헤어 위치
	FVector CameraLocation = FollowCamera->GetComponentLocation();
	FVector CameraForward = FollowCamera->GetForwardVector();

	// 카메라에서 앞쪽으로 Line Trace
	float TraceDistance = 10000.0f;
	FVector TraceEnd = CameraLocation + (CameraForward * TraceDistance);

	// Line Trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams))
	{
		return HitResult.Location;  // 맞은 위치
	}

	return TraceEnd;  // 아무것도 안 맞으면 끝점

}

FVector ABBBCharacterPlayer::GetCrosshairDirection() const
{
	if (!CurrentWeapon || !CurrentWeapon->WeaponMesh)
	{
		return GetActorForwardVector();
	}

	// 총구 위치
	FVector MuzzleLocation = CurrentWeapon->WeaponMesh->GetSocketLocation(FName("Muzzle"));

	// 크로스헤어 타겟 위치
	FVector CrosshairTarget = GetCrosshairWorldLocation();

	// 총구 → 크로스헤어 방향
	return (CrosshairTarget - MuzzleLocation).GetSafeNormal();
}

void ABBBCharacterPlayer::UpdatePlayerWidget()
{
	ABBBPlayerController* PC = Cast<ABBBPlayerController>(GetController());
	if (PC)
	{
		// HP 20 == 1 Heart
		int MaxHP = (StatComponent->MaxHP / 20);
		int CurrentHP = (StatComponent->CurrentHP / 20);

		PC->SetHP(MaxHP, CurrentHP);
	}
}

void ABBBCharacterPlayer::OnHPChangedCallback(float CurrentHP, float MaxHP, AActor* DamageCauser)
{
	UpdatePlayerWidget();
}