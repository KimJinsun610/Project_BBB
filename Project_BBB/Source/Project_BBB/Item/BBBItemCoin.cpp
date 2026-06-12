// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BBBItemCoin.h"
#include "Character/BBBCharacterPlayer.h"

ABBBItemCoin::ABBBItemCoin()
{
	Amount = 1;
}

void ABBBItemCoin::OnPickup_Implementation(AActor* Picker)
{
    Super::OnPickup_Implementation(Picker);

    ABBBCharacterPlayer* Player = Cast<ABBBCharacterPlayer>(Picker);
    if (!Player) return;

    Player->AddGold(Amount);

    // ∏‘¿∏∏È ªÁ∂Û¡¸
    Destroy();
}
