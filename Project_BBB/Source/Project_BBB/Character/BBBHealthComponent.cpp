// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BBBHealthComponent.h"
#include "Combat/BBBDebuffComponent.h"


UBBBHealthComponent::UBBBHealthComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	MaxHP = 100.f;
	CurrentHP = MaxHP;
	bIsDead = false;

}

void UBBBHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	
}

void UBBBHealthComponent::TakeDamage(float DamageAmount, AActor* DamageCauser)
{
	if (bIsDead) return;

	float FinalDamage = CalculateFinalDamage(DamageAmount);

	CurrentHP = FMath::Clamp(CurrentHP - FinalDamage, 0.f, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage (%.1f/%.1f HP)"),*GetOwner()->GetName(), FinalDamage, CurrentHP, MaxHP);

	OnHPChanged.Broadcast(CurrentHP, MaxHP, DamageCauser);

}

void UBBBHealthComponent::Heal(float HealAmount)
{
}

float UBBBHealthComponent::CalculateFinalDamage(float BaseDamage)
{
	return 0.0f;
}

float UBBBHealthComponent::GetHPPercent() const
{
	return 0.0f;
}


