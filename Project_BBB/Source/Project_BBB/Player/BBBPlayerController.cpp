// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BBBPlayerController.h"

void ABBBPlayerController::BeginPlay()
{

	Super::BeginPlay();


	// 시작하자 마자 포커스가 뷰포트 안에 들어가게 설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

}
