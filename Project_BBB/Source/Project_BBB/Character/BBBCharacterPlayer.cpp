// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BBBCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BBBCharacterControlData.h"
#include "Combat/BBBWeaponRanged.h"
#include "Combat/BBBWeaponMelee.h"

ABBBCharacterPlayer::ABBBCharacterPlayer()
{
	//================================================
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	//================================================
	// setup
	SetupCharacterMesh();


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

	// 원거리 
	static ConstructorHelpers::FClassFinder<ABBBWeaponBase> RangedWeaponClassRef( TEXT("/Game/BBB/BP/Wepones/BP_Gun.BP_Gun_C"));
	if (RangedWeaponClassRef.Class)
	{
		RangedWeaponClass = RangedWeaponClassRef.Class;
	}

	//근거리
	static ConstructorHelpers::FClassFinder<ABBBWeaponBase> MeleeWeaponClassRef(TEXT("/Game/BBB/BP/Wepones/BP_Sword.BP_Sword_C"));
	if (MeleeWeaponClassRef.Class)
	{
		MeleeWeaponClass = MeleeWeaponClassRef.Class;
	}
}

void ABBBCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	SetCharacterControl(CurrentCharacterControlType);

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
	
}

void ABBBCharacterPlayer::SetupCharacterMesh()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/BBB/Models/Player2/Player2_model.Player2_model'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/BBB/BP/Player/ABP_Player2.ABP_Player2_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

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
	SwitchWeapon();
}

void ABBBCharacterPlayer::PerformAttack(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Attack();
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
}

void ABBBCharacterPlayer::StopAttack(const FInputActionValue& Value)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopAttack();
	}
}
