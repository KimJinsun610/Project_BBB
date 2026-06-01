// BBBItemTypes.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BBBItemTypes.generated.h"

// =====================================================
// 아이템 타입 Enum
// =====================================================

UENUM(BlueprintType)
enum class EBBBItemType : uint8
{
    Currency    UMETA(DisplayName = "Currency"),    // 재화
    Consumable  UMETA(DisplayName = "Consumable"),  // 소비 아이템 (사과 등)
};

// =====================================================
// 아이템 데이터 (DataTable 행)
// Row Name = ItemID로 사용  ex) "Apple", "Coin"
// =====================================================

USTRUCT(BlueprintType)
struct FBBBItemData : public FTableRowBase
{
    GENERATED_BODY()

    // 표시 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText DisplayName;

    // 아이템 타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EBBBItemType ItemType = EBBBItemType::Consumable;

    // 인벤토리 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|UI")
    TObjectPtr<UTexture2D> Icon;

    // 최대 중첩 수량 (1이면 중첩 불가)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "1"))
    int32 MaxStackCount = 10;

    // [Consumable 전용] 회복 HP 량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Consumable")
    float HealAmount = 0.f;
};

// =====================================================
// 인벤토리 슬롯
// =====================================================

USTRUCT(BlueprintType)
struct FBBBInventorySlot
{
    GENERATED_BODY()

    // DataTable Row Name과 일치하는 아이템 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FName ItemID = NAME_None;

    // 보유 수량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Count = 0;

    FBBBInventorySlot() {}
    FBBBInventorySlot(FName InItemID, int32 InCount)
        : ItemID(InItemID), Count(InCount) {
    }

    bool IsValid() const { return ItemID != NAME_None && Count > 0; }
};


// =====================================================
// 퀵 슬롯
// =====================================================

USTRUCT(BlueprintType)
struct FBBBQuickSlotData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuickSlot")
    FName ItemID = NAME_None;

    FBBBQuickSlotData() {}
    FBBBQuickSlotData(FName InItemID) : ItemID(InItemID) {}

    bool IsEmpty() const { return ItemID == NAME_None; }
};