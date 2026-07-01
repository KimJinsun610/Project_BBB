// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item/BBBItemTypes.h"
#include "BBBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API UBBBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
    // 던전 누적 아이템 창고
    UPROPERTY(BlueprintReadWrite, Category = "Warehouse")
    TArray<FBBBInventorySlot> WarehouseSlots;

    // 보유 골드
    UPROPERTY(BlueprintReadWrite, Category = "Warehouse")
    int32 Gold = 0;

    // 던전 종료 시 아이템 누적 합산
    UFUNCTION(BlueprintCallable, Category = "Warehouse")
    void MergeInventory(const TArray<FBBBInventorySlot>& Slots);

    // 납품 완료 시 골드 추가
    UFUNCTION(BlueprintCallable, Category = "Warehouse")
    void AddGold(int32 Amount);

    // 창고에서 아이템 제거 (납품 시)
    UFUNCTION(BlueprintCallable, Category = "Warehouse")
    void RemoveFromWarehouse(FName ItemID, int32 Count);

    // 창고 초기화 (디버그용)
    UFUNCTION(BlueprintCallable, Category = "Warehouse")
    void ClearWarehouse();
};
