// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "BBBAudioManager.h"

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

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	TObjectPtr<UBBBAudioManager> AudioManager;

};
