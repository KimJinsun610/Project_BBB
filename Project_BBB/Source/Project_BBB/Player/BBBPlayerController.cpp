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

void ABBBPlayerController::SetGold(int32 InGold)
{
    if (HUDWidget)
    {
        UFunction* Func = HUDWidget->FindFunction(FName("SetGold"));
        if (Func)
        {
            struct FParams { int32 Gold; };
            FParams Params;
            Params.Gold = InGold;
            HUDWidget->ProcessEvent(Func, &Params);
        }
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
        UFunction* SetHPFunc = HUDWidget->FindFunction(FName("SetHP"));
        if (SetHPFunc)
        {
            struct FSetHPParams
            {
                int CurrentHP;
                int MaxHP;
            };

            FSetHPParams Params;
            Params.CurrentHP = iCurrentHP;
            Params.MaxHP = iMaxHP;

            HUDWidget->ProcessEvent(SetHPFunc, &Params);

        }

    }
}

void ABBBPlayerController::SetFireCooldown(float Percent)
{
    if (HUDWidget)
    {
        UFunction* Func = HUDWidget->FindFunction(FName("SetFireCooldown"));
        if (Func)
        {
            struct FParams { float Percent; };
            FParams Params;
            Params.Percent = Percent;
            HUDWidget->ProcessEvent(Func, &Params);
        }
    }
}
