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

    ABBBCharacterPlayer* Player = Cast<ABBBCharacterPlayer>(Picker);
    if (!Player) return;

    UBBBInventoryComponent* Inventory = Player->InventoryComponent;
    if (!Inventory) return;

    bool bAdded = Inventory->AddItem(FName("Apple"), Amount);

    UE_LOG(LogTemp, Warning, TEXT("Apple picked up! Added: %s"), bAdded ? TEXT("true") : TEXT("false"));

    Destroy();
}
