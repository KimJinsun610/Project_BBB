// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BBBItemBase.h"
#include "Components/SphereComponent.h"     

// Sets default values
ABBBItemBase::ABBBItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    PickupCollision->SetupAttachment(RootComponent);
    PickupCollision->SetSphereRadius(50.f);
    PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PickupCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    PickupCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ABBBItemBase::OnOverlapBegin);



}

void ABBBItemBase::OnPickup_Implementation(AActor* Picker)
{
}

void ABBBItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어만 체크

    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        OnPickup(OtherActor);
    }
}

