
#include "Player/BBBInventoryComponent.h"

#include "Character/BBBCharacterBase.h"
#include "Character/BBBStatComponent.h"

#include "Engine/DataTable.h"

#include "Kismet/GameplayStatics.h"

UBBBInventoryComponent::UBBBInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UBBBInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
    QuickSlots.SetNum(3);
	
}

bool UBBBInventoryComponent::AddItem(FName ItemID, int32 Count)
{
    if (ItemID == NAME_None || Count <= 0) {
        UE_LOG(LogTemp, Warning, TEXT("[Inventory] AddItem failed - invalid ID or Count"));
        return false;
    }

    FBBBItemData* Data = FindItemData(ItemID);
    if (!Data)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Inventory] AddItem failed - ItemData not found for: %s"), *ItemID.ToString());
        return false;
    }
    UE_LOG(LogTemp, Warning, TEXT("[Inventory] AddItem called - ItemID: %s, Count: %d"), *ItemID.ToString(), Count);

    int32 SlotIndex = FindSlotIndex(ItemID);

    if (SlotIndex != -1)
    {
        int32 NewCount = FMath::Min(Slots[SlotIndex].Count + Count, Data->MaxStackCount);
        int32 Remainder = (Slots[SlotIndex].Count + Count) - NewCount; // 초과분
        Slots[SlotIndex].Count = NewCount;

        // 초과분 있으면 새 슬롯에 추가
        if (Remainder > 0)
        {
            if (Slots.Num() >= MaxSlotCount)
            {
                OnInventoryChanged.Broadcast();
                return false; // 슬롯 꽉 참
            }
            Slots.Add(FBBBInventorySlot(ItemID, Remainder));
        }
    }
    else
    {
        if (Slots.Num() >= MaxSlotCount)
        {
            return false;
        }
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

        for (FBBBQuickSlotData& QSlot : QuickSlots)
        {
            if (QSlot.ItemID == ItemID)
            {
                QSlot = FBBBQuickSlotData();
            }
        }
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

    if (Data->ItemType == EBBBItemType::Consumable && Data->HealAmount > 0.f)
    {
        ABBBCharacterBase* Owner = Cast<ABBBCharacterBase>(GetOwner());
        if (Owner)
        {
            UBBBStatComponent* Stat = Owner->GetStatComponent();
            if (Stat->CurrentHP >= Stat->MaxHP) return false;
        }
    }

    ApplyItemEffect(*Data);
    RemoveItem(ItemID, 1);  // RemoveItem 안에서 OnInventoryChanged 호출됨

    return true;
}

int32 UBBBInventoryComponent::GetItemCount(FName ItemID) const
{
    int32 Index = FindSlotIndex(ItemID);
    return (Index != -1) ? Slots[Index].Count : 0;
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
        UBBBStatComponent* Stat = Owner->GetStatComponent();
        if (Stat->CurrentHP >= Stat->MaxHP) return;
        Stat->Heal(ItemData.HealAmount);

        if (UseItemSFX)
        {
            UGameplayStatics::PlaySound2D(GetWorld(), UseItemSFX, 0.4f);
        }
    }
}

bool UBBBInventoryComponent::GetItemData(FName ItemID, FBBBItemData& OutData) const
{
    FBBBItemData* Data = FindItemData(ItemID);
    if (!Data) return false;

    OutData = *Data;
    return true;
}

bool UBBBInventoryComponent::RegisterQuickSlot(FName ItemID)
{
    if (!HasItem(ItemID, 1)) return false;

    // 이미 등록된 슬롯이 있으면 갱신
    for (int32 i = 0; i < QuickSlots.Num(); i++)
    {
        if (QuickSlots[i].ItemID == ItemID)
        {
            OnQuickSlotChanged.Broadcast();
            return true;
        }
    }

    // 우측(2)부터 빈 슬롯 탐색
    for (int32 i = 0; i < QuickSlots.Num(); i++)
    {
        if (QuickSlots[i].IsEmpty())
        {
            QuickSlots[i] = FBBBQuickSlotData(ItemID);
            OnQuickSlotChanged.Broadcast();
            return true;
        }
    }

    return false; // 모든 슬롯이 가득 참
}

void UBBBInventoryComponent::RegisterQuickSlotAt(FName ItemID, int32 SlotIndex)
{
    if (!QuickSlots.IsValidIndex(SlotIndex)) return;
    if (!HasItem(ItemID, 1)) return;

    QuickSlots[SlotIndex] = FBBBQuickSlotData(ItemID);
    OnQuickSlotChanged.Broadcast();
}

bool UBBBInventoryComponent::UseQuickSlot(int32 SlotIndex)
{
    if (!QuickSlots.IsValidIndex(SlotIndex)) return false;
    if (QuickSlots[SlotIndex].IsEmpty()) return false;

    FName ItemID = QuickSlots[SlotIndex].ItemID;
    return UseItem(ItemID); // UseItem 내부에서 OnInventoryChanged Broadcast
}