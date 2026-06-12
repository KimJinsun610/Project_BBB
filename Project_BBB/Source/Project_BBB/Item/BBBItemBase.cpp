// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BBBItemBase.h"
#include "Components/SphereComponent.h"     
#include "Kismet/GameplayStatics.h"

// Sets default values
ABBBItemBase::ABBBItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

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


void ABBBItemBase::BeginPlay()
{
    Super::BeginPlay();
    InitialLocation = GetActorLocation();
}

void ABBBItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    FloatTime += DeltaTime * FloatSpeed;

    FVector NewLocation = InitialLocation;
    NewLocation.Z += FMath::Sin(FloatTime) * FloatAmplitude;

    SetActorLocation(NewLocation);
    
}


void ABBBItemBase::OnPickup_Implementation(AActor* Picker)
{
    if (PickupSFX)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), PickupSFX, 0.15f, 1.5f);
    }
}

void ABBBItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        OnPickup(OtherActor);
    }
}



