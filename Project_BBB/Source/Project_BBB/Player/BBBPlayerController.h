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

protected:
    
    //HUD 위젯
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* HUDWidget;

public:
    // C++에서 구현, 블루프린트에서 호출 가능
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowCrosshair(bool bShow);
};
