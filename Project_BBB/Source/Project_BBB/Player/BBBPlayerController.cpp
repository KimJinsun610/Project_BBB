// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/BBBPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Character/BBBCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BBBInventoryComponent.h"

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

    if (bIsInventoryOpen && InventoryWidget)
    {
        UFunction* JemFunc = InventoryWidget->FindFunction(FName("UpdateJem"));
        if (JemFunc)
        {
            struct FGoldParams { int32 JemAmount; };
            FGoldParams Params;
            Params.JemAmount = InGold;
            InventoryWidget->ProcessEvent(JemFunc, &Params);
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

void ABBBPlayerController::SetFireCooldown(float Duration)
{
    if (HUDWidget)
    {
        UFunction* Func = HUDWidget->FindFunction(FName("SetFireCooldown"));

        if (Func)
        {
            struct FCooldownParams { double Duration; };
            FCooldownParams Params;
            Params.Duration = Duration;
            HUDWidget->ProcessEvent(Func, &Params);
        }
    }
}

void ABBBPlayerController::ShowGameOverUI(float SurvivalTime)
{
    if (!GameOverWidgetClass) return;

    UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
    if (GameOverWidget)
    {
        GameOverWidget->AddToViewport(10); // HUD보다 위에 표시

        ABBBCharacterPlayer* ownPlayer = Cast<ABBBCharacterPlayer>(GetPawn());
        UBBBInventoryComponent* InvComp = ownPlayer ? ownPlayer->InventoryComponent : nullptr;

        UFunction* Func = GameOverWidget->FindFunction(FName("InitGameOverUI"));
        if (Func)
        {
            struct FParams
            {
                double SurvivalTime;
                UBBBInventoryComponent* InventoryComponent;
            };
            FParams Params;
            Params.SurvivalTime = SurvivalTime;
            Params.InventoryComponent = InvComp;
            GameOverWidget->ProcessEvent(Func, &Params);
        }

        bShowMouseCursor = true;
        FInputModeUIOnly InputMode;
        SetInputMode(InputMode);
    }
}

void ABBBPlayerController::ToggleInventory()
{
    FString CurrentLevel = GetWorld()->GetMapName();
    if (CurrentLevel.Contains(TEXT("BBB_Lob")))  return;


    // 위젯 최초 생성
    if (!InventoryWidget && InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
    }
    if (!InventoryWidget) return;

    bIsInventoryOpen = !bIsInventoryOpen;

    if (bIsInventoryOpen)
    {
        // 조준 중엔 인벤토리 사용 불가
        ABBBCharacterPlayer* PlayerChar = Cast<ABBBCharacterPlayer>(GetPawn());
        if (PlayerChar) PlayerChar->ForceStopAim();

        InventoryWidget->AddToViewport();

        UFunction* Func = InventoryWidget->FindFunction(FName("RefreshInventory"));
        if (Func)
        {
            InventoryWidget->ProcessEvent(Func, nullptr);
        }

        if (PlayerChar)
        {
            UFunction* JemFunc = InventoryWidget->FindFunction(FName("UpdateJem"));
            if (JemFunc)
            {
                struct FGoldParams { int32 JemAmount; };
                FGoldParams Params;
                Params.JemAmount = PlayerChar->GetGold();
                InventoryWidget->ProcessEvent(JemFunc, &Params);
            }

            if (InventoryOpenSFX)
            {
                UGameplayStatics::PlaySound2D(GetWorld(), InventoryOpenSFX, 0.5f, 1.3f);
            }

            // 마우스 커서 표시 + UI 입력 허용
            bShowMouseCursor = true;
            FInputModeGameAndUI InputMode;
            InputMode.SetHideCursorDuringCapture(false);
            SetInputMode(InputMode);
        }
    }
    else
    {
        UFunction* ClearFunc = InventoryWidget->FindFunction(FName("ClearSelection"));
        if (ClearFunc) InventoryWidget->ProcessEvent(ClearFunc, nullptr);

        InventoryWidget->RemoveFromParent();

        if (InventoryCloseSFX)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), InventoryCloseSFX, 0.5f, 1.3f);
        }

        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());
    }
 }
