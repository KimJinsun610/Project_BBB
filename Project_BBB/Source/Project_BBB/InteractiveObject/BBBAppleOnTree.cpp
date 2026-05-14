// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject/BBBAppleOnTree.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"


FTimerHandle LandingDetectTimer;

ABBBAppleOnTree::ABBBAppleOnTree()
{
    // AI°¡ ÀÚµ¿À¸·Î ºùÀÇ X
    AutoPossessAI = EAutoPossessAI::Disabled;

    // »ç°ú ¸Þ½Ã ÄÄÆ÷³ÍÆ®
    AppleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AppleMesh"));
    AppleMesh->SetupAttachment(RootComponent);
    AppleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABBBAppleOnTree::BeginPlay()
{
    Super::BeginPlay(); 

    GetCharacterMovement()->DisableMovement();
    GetCharacterMovement()->GravityScale = 0.0f;

    // À§Á¬ ¼û±è
    if (EnemyInfoWidgetComponent)
    {
        EnemyInfoWidgetComponent->SetVisibility(false);
    }
}

//=======================================================================
// IBBBHittableObject 

void ABBBAppleOnTree::OnHitByProjectile(AActor* Projectile)
{
    if (bIsFalling) return; // Áßº¹ È£Ãâ ¹æÁö
    bIsFalling = true;

    StartFalling();
}

//=======================================================================
// ³«ÇÏ ½ÃÄö½º 

void ABBBAppleOnTree::StartFalling()
{
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->SetComponentTickEnabled(false);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (AppleMesh)
    {
        AppleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        AppleMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
        AppleMesh->SetSimulatePhysics(true);

        GetWorld()->GetTimerManager().SetTimer(
            LandingDetectTimer,
            [this]()
            {
                if (AppleMesh && !bHasLanded)
                    AppleMesh->OnComponentHit.AddDynamic(
                        this, &ABBBAppleOnTree::OnAppleLanded);
            },
            0.1f, false
        );
    }

    GetWorld()->GetTimerManager().SetTimer(
        FallSafetyTimer,
        [this]() { SpawnItemAndDestroy(); },
        3.0f, false
    );
}

void ABBBAppleOnTree::OnAppleLanded(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bHasLanded) return;
    bHasLanded = true;

    GetWorld()->GetTimerManager().ClearTimer(FallSafetyTimer);
    SpawnItemAndDestroy();
}

void ABBBAppleOnTree::SpawnItemAndDestroy()
{
    if (AppleMesh)
    {
        SetActorLocation(AppleMesh->GetComponentLocation());
    }

    DropItems();
    Destroy();
}