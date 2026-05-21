 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BBBItemBase.generated.h"

UCLASS()
class PROJECT_BBB_API ABBBItemBase : public AActor
{
	GENERATED_BODY()
public:
    ABBBItemBase();

    // 플레이어가 먹었을 때 호출
    UFUNCTION(BlueprintNativeEvent, Category = "Item")
    void OnPickup(AActor* Picker);

protected:

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* PickupCollision;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
                        AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Amount;

public:
    void SetAmount(int32 InAmount) { Amount = InAmount; }


protected:
    UPROPERTY(EditAnywhere, Category = "Item|Float")
    float FloatAmplitude = 5.f;   

    UPROPERTY(EditAnywhere, Category = "Item|Float")
    float FloatSpeed = 2.f;        

private:
    FVector InitialLocation;
    float FloatTime = 0.f;         
};
