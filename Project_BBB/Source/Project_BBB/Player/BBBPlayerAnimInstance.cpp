// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BBBPlayerAnimInstance.h"
#include "BBBPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBBBPlayerAnimInstance::UBBBPlayerAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 10.0f;
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
}
