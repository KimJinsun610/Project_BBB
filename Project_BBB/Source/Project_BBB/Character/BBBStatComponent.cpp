// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BBBStatComponent.h"
#include "Combat/BBBDebuffComponent.h"


UBBBStatComponent::UBBBStatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	MaxHP = 100.f;
	CurrentHP = MaxHP;
	bIsDead = false;

	AttackRadius = 50.f;

}

void UBBBStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;
	
}

void UBBBStatComponent::TakeDamage(float DamageAmount, AActor* DamageCauser)
{
	if (bIsDead) return;

	float FinalDamage = CalculateFinalDamage(DamageAmount);

	CurrentHP = FMath::Clamp(CurrentHP - FinalDamage, 0.f, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage (%.1f/%.1f HP)"),*GetOwner()->GetName(), FinalDamage, CurrentHP, MaxHP);

	OnHPChanged.Broadcast(CurrentHP, MaxHP, DamageCauser);

	if (CurrentHP <= 0.0f)
	{
		bIsDead = true;
		OnDeath.Broadcast(GetOwner(), DamageCauser);

		UE_LOG(LogTemp, Warning, TEXT("%s is dead!"), *GetOwner()->GetName());
	}
}

void UBBBStatComponent::Heal(float HealAmount)
{
	if (bIsDead)
	{
		return;
	}

	CurrentHP = FMath::Clamp(CurrentHP + HealAmount, 0.0f, MaxHP);
	OnHPChanged.Broadcast(CurrentHP, MaxHP, nullptr);

	UE_LOG(LogTemp, Log, TEXT("%s healed %.1f HP (%.1f/%.1f)"), *GetOwner()->GetName(), HealAmount, CurrentHP, MaxHP);
}

float UBBBStatComponent::CalculateFinalDamage(float BaseDamage)
{
	float FinalDamage = BaseDamage;

	UBBBDebuffComponent* DebuffComp = GetOwner()->FindComponentByClass<UBBBDebuffComponent>();
	if (DebuffComp && DebuffComp->HasDebuff(EDebuffType::Weaken))
	{
		FinalDamage *= 1.5f;  // 50% 추가 데미지
		UE_LOG(LogTemp, Warning, TEXT("Weaken bonus damage: +50%%"));
	}


	return FinalDamage;
}

float UBBBStatComponent::GetHPPercent() const
{
	if (MaxHP <= 0.0f)
	{
		return 0.0f;
	}

	return CurrentHP / MaxHP;
}


