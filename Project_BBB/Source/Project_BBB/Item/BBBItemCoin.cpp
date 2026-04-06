// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BBBItemCoin.h"
#include "Character/BBBCharacterPlayer.h"

ABBBItemCoin::ABBBItemCoin()
{
	Amount = 1;
}

void ABBBItemCoin::OnPickup_Implementation(AActor* Picker)
{
    ABBBCharacterPlayer* Player = Cast<ABBBCharacterPlayer>(Picker);
    if (!Player) return;

    // 나중에 플레이어 골드 시스템 연결
    UE_LOG(LogTemp, Warning, TEXT("Coin picked up! Amount: %d"), Amount);

    // 먹으면 사라짐
    Destroy();
}
