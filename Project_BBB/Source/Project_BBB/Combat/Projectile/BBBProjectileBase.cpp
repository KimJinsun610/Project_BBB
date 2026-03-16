// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Projectile/BBBProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABBBProjectileBase::ABBBProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

    // 기본값 설정
    Speed = 3000.0f;
    LifeSpan = 10.0f;
    CollisionRadius = 15.0f;

    // 충돌 컴포넌트
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    CollisionComponent->InitSphereRadius(CollisionRadius);
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = CollisionComponent;

    // 충돌 이벤트 바인딩
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABBBProjectileBase::OnHit);

    // 메시 컴포넌트
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(CollisionComponent);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 발사체 무브먼트
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;  // 중력 영향 X
    UE_LOG(LogTemp, Warning, TEXT("PostInit - Speed: %f"), Speed);

    // 수명 설정
    InitialLifeSpan = LifeSpan;
}

void ABBBProjectileBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (ProjectileMovement)
    {
        ProjectileMovement->InitialSpeed = Speed;
        ProjectileMovement->MaxSpeed = Speed;

        ProjectileMovement->Velocity = GetActorForwardVector() * Speed;
    }
}

void ABBBProjectileBase::BeginPlay()
{
	Super::BeginPlay();

    // 플레이어 충돌 무시
    if (CollisionComponent && GetOwner())
    {
        CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    }

    // !!!! 궤적 이펙트 (추후 확인) !!!!
    if (TrailEffect)
    {
        UGameplayStatics::SpawnEmitterAttached(
            TrailEffect,
            MeshComponent,
            NAME_None,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            EAttachLocation::KeepRelativeOffset,
            true
        );
    }
	
}

void ABBBProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 자기 자신이거나 발사자면 무시
    if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
    {
        return;
    }

    // 충돌 이펙트 생성
    SpawnImpactEffect(Hit.ImpactPoint);

    // 자식 클래스의 처리 호출
    OnProjectileHit(OtherActor, Hit);

    // 발사체 제거
    Destroy();

}

void ABBBProjectileBase::OnProjectileHit(AActor* HitActor, const FHitResult& Hit)
{ 
    // 자식 클래스에서 수행
}

void ABBBProjectileBase::SpawnImpactEffect(const FVector& ImpactLocation)
{
    if (ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ImpactEffect,
            ImpactLocation,
            FRotator::ZeroRotator,
            true
        );
    }
}
