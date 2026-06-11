// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BBBAudioManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BBB_API UBBBAudioManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UBBBAudioManager();

	UPROPERTY(EditDefaultsOnly, Category = "BGM")
	TObjectPtr<USoundBase> LevelBGM;

	void PlayBGM(USoundBase* BGM);
	void StopBGM();


	// UI SFX
	UPROPERTY(EditDefaultsOnly, Category = "SFX|UI")
	TObjectPtr<USoundBase> UIClickSFX;

	UFUNCTION(BlueprintCallable, Category = "SFX|UI")
	void PlayUIClickSFX();


private:
	UPROPERTY(Transient)
	TObjectPtr<UAudioComponent> BGMAudioComponent;

		
};
