// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Projectile/BBBProjectileBase.h"
#include "Combat/BBBDebuffTypes.h"
#include "BBBProjectileDebuff.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBProjectileDebuff : public ABBBProjectileBase
{
	GENERATED_BODY()

public:
	ABBBProjectileDebuff();
	void Initialize(const FDebuffData& InDebuffData);

protected:
    virtual void OnProjectileHit(AActor* HitActor, const FHitResult& Hit) override;

    // 적용할 디버프
    UPROPERTY(EditAnywhere, Category = "Debuff")
    FDebuffData DebuffData;

    // 디버프 타입별 색상
    void SetProjectileColor();
};
