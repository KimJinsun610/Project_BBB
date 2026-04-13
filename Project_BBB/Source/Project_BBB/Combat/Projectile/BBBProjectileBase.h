#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBBProjectileBase.generated.h"

UCLASS()
class PROJECT_BBB_API ABBBProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABBBProjectileBase();

    virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

    // 충돌 감지
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    class USphereComponent* CollisionComponent;

    // 발사체 물리 (날아가는 기능)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    class UProjectileMovementComponent* ProjectileMovement;

    // 메쉬
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    class UStaticMeshComponent* MeshComponent;


    // === 공통 설정 값 ===

    UPROPERTY(EditAnywhere, Category = "Projectile Stats")
    float Speed;  // 발사 속도

    UPROPERTY(EditAnywhere, Category = "Projectile Stats")
    float LifeSpan;  // 수명 (날아가는 시간)

    UPROPERTY(EditAnywhere, Category = "Projectile Stats")
    float CollisionRadius;  // 충돌 반지름


    UPROPERTY(EditAnywhere, Category = "Projectile Stats")
    float Damage = 0.f; // 데미지

    UPROPERTY(EditAnywhere, Category = "Projectile Stats")
    bool bDamageOnHit = false; // true면 맞은 액터에 데미지


    // === 충돌 ===

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    // 자식 클래스에서 오버라이드할 함수
    virtual void OnProjectileHit(AActor* HitActor, const FHitResult& Hit);


    // === 시각 효과 ===

    UPROPERTY(EditAnywhere, Category = "Projectile Effects")
    class UParticleSystem* TrailEffect;  // 궤적 이펙트

    UPROPERTY(EditAnywhere, Category = "Projectile Effects")
    class UParticleSystem* ImpactEffect;  // 충돌 이펙트

    void SpawnImpactEffect(const FVector& ImpactLocation);


};
