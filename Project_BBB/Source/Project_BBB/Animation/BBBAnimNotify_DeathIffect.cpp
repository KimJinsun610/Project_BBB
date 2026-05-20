// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BBBAnimNotify_DeathIffect.h"

#include "Character/Enemy/BBBEnemyBase.h"

void UBBBAnimNotify_DeathIffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    ABBBEnemyBase* Enemy = Cast<ABBBEnemyBase>(MeshComp->GetOwner());
    if (Enemy)
    {
        Enemy->OnDeathEffectNotify();
    }
}
