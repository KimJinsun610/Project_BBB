
#include "Player/BBBInventoryComponent.h"
#include "Character/BBBCharacterBase.h"
#include "Character/BBBStatComponent.h"
#include "Engine/DataTable.h"

UBBBInventoryComponent::UBBBInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UBBBInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool UBBBInventoryComponent::AddItem(FName ItemID, int32 Count)
{
    if (ItemID == NAME_None || Count <= 0) return false;

    FBBBItemData* Data = FindItemData(ItemID);
    if (!Data) return false;

    int32 SlotIndex = FindSlotIndex(ItemID);

    if (SlotIndex != -1)
    {
        // 기존 슬롯에 수량 추가 (MaxStackCount 초과 방지)
        int32 NewCount = FMath::Min(Slots[SlotIndex].Count + Count, Data->MaxStackCount);
        Slots[SlotIndex].Count = NewCount;
    }
    else
    {
        // 새 슬롯 추가
        int32 ClampedCount = FMath::Min(Count, Data->MaxStackCount);
        Slots.Add(FBBBInventorySlot(ItemID, ClampedCount));
    }

    OnInventoryChanged.Broadcast();
    return true;
}

bool UBBBInventoryComponent::RemoveItem(FName ItemID, int32 Count)
{
    int32 SlotIndex = FindSlotIndex(ItemID);
    if (SlotIndex == -1) return false;

    Slots[SlotIndex].Count -= Count;

    if (Slots[SlotIndex].Count <= 0)
    {
        Slots.RemoveAt(SlotIndex);
    }

    OnInventoryChanged.Broadcast();
    return true;
}

// =====================================================
// Hold Check
bool UBBBInventoryComponent::HasItem(FName ItemID, int32 Count) const
{
    int32 SlotIndex = FindSlotIndex(ItemID);
    if (SlotIndex == -1) return false;

    return Slots[SlotIndex].Count >= Count;
}

// =====================================================
// Item Use
bool UBBBInventoryComponent::UseItem(FName ItemID)
{
    if (!HasItem(ItemID, 1)) return false;

    FBBBItemData* Data = FindItemData(ItemID);
    if (!Data) return false;

    ApplyItemEffect(*Data);
    RemoveItem(ItemID, 1);  // RemoveItem 안에서 OnInventoryChanged 호출됨

    return true;
}

// =====================================================
// Utilety
FBBBItemData* UBBBInventoryComponent::FindItemData(FName ItemID) const
{
    if (!ItemDataTable) return nullptr;
    return ItemDataTable->FindRow<FBBBItemData>(ItemID, TEXT("FindItemData"));
}

int32 UBBBInventoryComponent::FindSlotIndex(FName ItemID) const
{
    for (int32 i = 0; i < Slots.Num(); i++)
    {
        if (Slots[i].ItemID == ItemID) return i;
    }
    return -1;
}

void UBBBInventoryComponent::ApplyItemEffect(const FBBBItemData& ItemData)
{
    if (ItemData.ItemType != EBBBItemType::Consumable) return;

    ABBBCharacterBase* Owner = Cast<ABBBCharacterBase>(GetOwner());
    if (!Owner) return;

    // HealAmount가 있으면 체력 회복
    if (ItemData.HealAmount > 0.f)
    {
        Owner->GetStatComponent()->Heal(ItemData.HealAmount);
    }
}

bool UBBBInventoryComponent::GetItemData(FName ItemID, FBBBItemData& OutData) const
{
    FBBBItemData* Data = FindItemData(ItemID);
    if (!Data) return false;

    OutData = *Data;
    return true;
}
