// Fill out your copyright notice in the Description page of Project Settings.

#include "ColonyBuilderGameModeBase.h"
#include "RTSPlayerController.h"
#include "HUD/RTSHUD.h"


AColonyBuilderGameModeBase::AColonyBuilderGameModeBase()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
	PlayerControllerClass = ARTSPlayerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
}
