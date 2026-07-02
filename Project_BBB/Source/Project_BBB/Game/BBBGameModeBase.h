// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "BBBAudioManager.h"
#include "Item/BBBItemTypes.h" 

#include "BBBGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_BBB_API ABBBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABBBGameModeBase();

	UFUNCTION(BlueprintCallable, Category = "Audio")
	UBBBAudioManager* GetAudioManager() const { return AudioManager; }

	// 플레이어 사망 처리
	UFUNCTION()
	void OnPlayerDead(const TArray<FBBBInventorySlot>& EarnedSlots, int32 EarnedGold);


	UFUNCTION(BlueprintCallable, Category = "Game")
	void ConfirmReturnToLobby(const TArray<FBBBInventorySlot>& EarnedSlots, int32 EarnedGold);

	UFUNCTION(BlueprintPure, Category = "Game")
	float GetSurvivalTime() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	TObjectPtr<UBBBAudioManager> AudioManager;

	float GameStartTime = 0.0f;
};
