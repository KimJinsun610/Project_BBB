// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BBBGameModeBase.h"
#include "Player/BBBPlayerController.h"

ABBBGameModeBase::ABBBGameModeBase()
{
	//DefaultPawnClass

	//PlayerControllerClass
	PlayerControllerClass = ABBBPlayerController::StaticClass();

}
