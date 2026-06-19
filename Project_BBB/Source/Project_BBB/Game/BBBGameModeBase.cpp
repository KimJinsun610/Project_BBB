// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BBBGameModeBase.h"

#include "Player/BBBPlayerController.h"
#include "Kismet/GameplayStatics.h" 

ABBBGameModeBase::ABBBGameModeBase()
{
	//DefaultPawnClass
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/BBB/Actors/Player/BP_Player.BP_Player_C"));
	if (ThirdPersonClassRef.Class) {
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}



	//PlayerControllerClass
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Project_BBB.BBBPlayerController"));

	if (PlayerControllerClassRef.Class) {
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}


	AudioManager = CreateDefaultSubobject<UBBBAudioManager>(TEXT("AudioManager"));
}

void ABBBGameModeBase::OnPlayerDead()
{
	ABBBPlayerController* PC = Cast<ABBBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC)
	{
		PC->ShowGameOverUI();
	}
}

// BeginPlay
void ABBBGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    AudioManager->PlayBGM(AudioManager->LevelBGM);
}