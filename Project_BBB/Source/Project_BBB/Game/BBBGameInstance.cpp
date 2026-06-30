// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BBBGameInstance.h"

void UBBBGameInstance::MergeInventory(const TArray<FBBBInventorySlot>& Slots)
{
    for (const FBBBInventorySlot& NewSlot : Slots)
    {
        if (!NewSlot.IsValid()) continue;

        bool bFound = false;
        for (FBBBInventorySlot& Existing : WarehouseSlots)
        {
            if (Existing.ItemID == NewSlot.ItemID)
            {
                Existing.Count += NewSlot.Count;
                bFound = true;
                break;
            }
        }

        if (!bFound)
            WarehouseSlots.Add(NewSlot);
    }
}

void UBBBGameInstance::AddGold(int32 Amount)
{
    Gold += Amount;
}

void UBBBGameInstance::RemoveFromWarehouse(FName ItemID, int32 Count)
{
    for (int32 i = 0; i < WarehouseSlots.Num(); i++)
    {
        if (WarehouseSlots[i].ItemID == ItemID)
        {
            WarehouseSlots[i].Count -= Count;
            if (WarehouseSlots[i].Count <= 0)
                WarehouseSlots.RemoveAt(i);
            return;
        }
    }
}

void UBBBGameInstance::ClearWarehouse()
{
    WarehouseSlots.Empty();
}