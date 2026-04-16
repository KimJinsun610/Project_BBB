// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BBBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

//================================================
// Item Section

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetGold(int32 InGold);
//================================================
// UI Section

protected:
    //HUD 위젯
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* HUDWidget;

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowCrosshair(bool bShow);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowWeaponInfo(bool bIsRange);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetHP(int iMaxHP, int iCurrentHP);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetFireCooldown(float Percent);
};
