// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/BBBEnemyBase.h"
#include "Character/Enemy/BBBHittableInterface.h"
#include "BBBAppleOnTree.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBAppleOnTree : public ABBBEnemyBase, public IBBBHittableInterface
{
	GENERATED_BODY()
	
public:
	ABBBAppleOnTree();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Apple")
    TObjectPtr<UStaticMeshComponent> AppleMesh;

    // 땅에 떨어지는 사운드
    UPROPERTY(EditDefaultsOnly, Category = "SFX")
    TObjectPtr<USoundBase> LandSFX;

public:
    // IBBBHittableObject 
    virtual void OnHitByProjectile(AActor* Projectile) override;

private:
    void StartFalling();

    UFUNCTION()
    void OnAppleLanded(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    void SpawnItemAndDestroy();

    bool bIsFalling = false;
    bool bHasLanded = false;

    FTimerHandle FallSafetyTimer;
    FTimerHandle LandingDetectTimer;
};
