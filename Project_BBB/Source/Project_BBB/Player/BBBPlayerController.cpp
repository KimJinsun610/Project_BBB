// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/BBBPlayerController.h"
#include "Blueprint/UserWidget.h"

void ABBBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 전용 입력 모드
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// HUD 위젯 생성
    if (HUDWidgetClass)
    {
        HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidget)
        {
            HUDWidget->AddToViewport();

            //초기에 크로스헤어 숨김 
            ShowCrosshair(false);

            //원거리로 시작
            ShowWeaponInfo(true);

            UE_LOG(LogTemp, Warning, TEXT("HUD Widget Created and Added to Viewport"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create HUD Widget!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HUDWidgetClass is NULL!"));
    }
}

void ABBBPlayerController::ShowCrosshair(bool bShow)
{
    if (HUDWidget)
    {
        // 블루프린트 함수 호출
        UFunction* ShowCrosshairFunc = HUDWidget->FindFunction(FName("ShowCrosshair"));
        if (ShowCrosshairFunc)
        {
            struct FShowCrosshairParams
            {
                bool bShow;
            };

            FShowCrosshairParams Params;
            Params.bShow = bShow;

            HUDWidget->ProcessEvent(ShowCrosshairFunc, &Params);

        }
 
    }
  
}

void ABBBPlayerController::ShowWeaponInfo(bool bIsRange)
{
    if (HUDWidget)
    {
        // 블루프린트 함수 호출
        UFunction* ShowCrosshairFunc = HUDWidget->FindFunction(FName("ShowWeaponInfo"));
        UE_LOG(LogTemp, Warning, TEXT("Show UI"));
        if (ShowCrosshairFunc)
        {
            struct FShowWeaponInfoParams
            {
                bool bShow;
            };

            FShowWeaponInfoParams Params;
            Params.bShow = bIsRange;

            HUDWidget->ProcessEvent(ShowCrosshairFunc, &Params);

        }

    }
}

void ABBBPlayerController::SetHP(int iMaxHP, int iCurrentHP)
{
    if (HUDWidget)
    {
        // 블루프린트 함수 호출
        UFunction* ShowCrosshairFunc = HUDWidget->FindFunction(FName("SetHP"));
        UE_LOG(LogTemp, Warning, TEXT("Show UI"));
        if (ShowCrosshairFunc)
        {
            struct FSetHPParams
            {
                int CurrentHP;
                int MaxHP;
            };

            FSetHPParams Params;
            Params.CurrentHP = iCurrentHP;
            Params.MaxHP = iMaxHP;

            HUDWidget->ProcessEvent(ShowCrosshairFunc, &Params);

        }

    }
}
