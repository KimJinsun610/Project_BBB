// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BBBGameModeBase.h"
#include "Player/BBBPlayerController.h"

ABBBGameModeBase::ABBBGameModeBase()
{
	//DefaultPawnClass
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Script/Project_BBB.BBBCharacterPlayer"));
	if (ThirdPersonClassRef.Class) {
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}



	//PlayerControllerClass
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/Project_BBB.BBBPlayerController"));

	if (PlayerControllerClassRef.Class) {
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

}
