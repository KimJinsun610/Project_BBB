// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BBBPlayerAnimInstance.h"
#include "BBBPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBBBPlayerAnimInstance::UBBBPlayerAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 10.0f;

	bIsAiming = false;
	bIsShooting = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShootingMontageRef(
		TEXT("/Game/BBB/Characters/Player/AM_Shooting.AM_Shooting")
	);
	if (ShootingMontageRef.Object)
	{
		ShootingMontage = ShootingMontageRef.Object;
	}
}

void UBBBPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	//해당 Class를 소유하고 있는 액터에 관한 정보
	Owner = Cast<ACharacter>(GetOwningActor());

	//캐릭터 무브먼트에 관한 정보
	if (Owner) {
		Movement = Owner->GetCharacterMovement();
	}
}

void UBBBPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Movement) {
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling | (Velocity.Z > JumpingThreshold);
		bIsGrounded = ~bIsFalling;
	}

	if (Owner && bIsAiming)
	{
		FRotator AimRotation = Owner->GetBaseAimRotation();
		FRotator ActorRotation = Owner->GetActorRotation();

		FRotator DeltaRotation = AimRotation - ActorRotation;
		DeltaRotation.Normalize();

	}
}

void UBBBPlayerAnimInstance::PlayShootingAnimation()
{
	if (ShootingMontage)
	{
		Montage_Play(ShootingMontage, 1.0f);
		bIsShooting = true;

		FTimerHandle ShootingTimer;
		float MontageLength = ShootingMontage->GetPlayLength();

		GetWorld()->GetTimerManager().SetTimer(
			ShootingTimer,
			[this]()
			{
				bIsShooting = false;
			},
			MontageLength,
			false
		);
	}
}

void UBBBPlayerAnimInstance::SetAiming(bool bAiming)
{
	bIsAiming = bAiming;

	if (bAiming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Started Aiming"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped Aiming"));
	}
}
