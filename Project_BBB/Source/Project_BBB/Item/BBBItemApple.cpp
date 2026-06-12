// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BBBItemApple.h"
#include "Character/BBBCharacterPlayer.h"
#include "Player/BBBInventoryComponent.h"

ABBBItemApple::ABBBItemApple()
{
	Amount = 1;
}

void ABBBItemApple::OnPickup_Implementation(AActor* Picker)
{

    Super::OnPickup_Implementation(Picker);

    ABBBCharacterPlayer* Player = Cast<ABBBCharacterPlayer>(Picker);
    if (!Player) return;

    UBBBInventoryComponent* Inventory = Player->InventoryComponent;
    if (!Inventory) return;

    bool bAdded = Inventory->AddItem(FName("Apple"), Amount);
    
    if (bAdded)
    {
        Destroy();
    }
}
