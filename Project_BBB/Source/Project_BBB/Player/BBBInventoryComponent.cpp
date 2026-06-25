
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

TArray<FBBBInventorySlot> UBBBInventoryComponent::GetMergedSlots() const
{
    TArray<FBBBInventorySlot> Merged;

    for (const FBBBInventorySlot& Slot : Slots)
    {
        if (Slot.Count <= 0) continue;

        FBBBInventorySlot* Existing = Merged.FindByPredicate(
            [&](const FBBBInventorySlot& S) { return S.ItemID == Slot.ItemID; });

        if (Existing)
            Existing->Count += Slot.Count;
        else
            Merged.Add(Slot);
    }

    return Merged;
}

bool UBBBInventoryComponent::AddItem(FName ItemID, int32 Count)
{
    if (ItemID == NAME_None || Count <= 0) return false;

    FBBBItemData* Data = FindItemData(ItemID);
    if (!Data) return false;

    int32 Remaining = Count;

    for (int32 i = 0; i < Slots.Num() && Remaining > 0; i++)
    {
        if (Slots[i].ItemID != ItemID) continue;
        if (Slots[i].Count >= Data->MaxStackCount) continue;

        int32 Space = Data->MaxStackCount - Slots[i].Count;
        int32 ToAdd = FMath::Min(Space, Remaining);
        Slots[i].Count += ToAdd;
        Remaining -= ToAdd;
    }

    while (Remaining > 0)
    {
        if (Slots.Num() >= MaxSlotCount) break;

        int32 ToAdd = FMath::Min(Remaining, Data->MaxStackCount);
        Slots.Add(FBBBInventorySlot(ItemID, ToAdd));
        Remaining -= ToAdd;
    }

    OnInventoryChanged.Broadcast();
    return (Remaining == 0);
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
    int32 Total = 0;
    for (const FBBBInventorySlot& Slot : Slots)
    {
        if (Slot.ItemID == ItemID) Total += Slot.Count;
    }
    return Total;
}

bool UBBBInventoryComponent::UseItemAt(int32 SlotIndex)
{
    if (!Slots.IsValidIndex(SlotIndex)) return false;

    FName ItemID = Slots[SlotIndex].ItemID;
    FBBBItemData* Data = FindItemData(ItemID);
    if (!Data) return false;

    // 소모품 HP 체크
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

    // 해당 슬롯에서만 1개 차감
    Slots[SlotIndex].Count -= 1;
    if (Slots[SlotIndex].Count <= 0)
    {
        Slots.RemoveAt(SlotIndex);

        for (FBBBQuickSlotData& QSlot : QuickSlots)
        {
            if (QSlot.ItemID == ItemID && !HasItem(ItemID, 1))
                QSlot = FBBBQuickSlotData();
        }
    }

    OnInventoryChanged.Broadcast();
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
    for (int32 i = Slots.Num()-1 ; i >= 0 ; i--)
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