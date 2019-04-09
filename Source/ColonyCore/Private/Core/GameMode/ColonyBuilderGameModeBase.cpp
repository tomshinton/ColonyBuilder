// Fill out your copyright notice in the Description page of Project Settings.

#include "ColonyBuilderGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Utils/Libraries/ManagerUtils.h"
#include "UI/HUD/RTSHUD.h"
#include "../Player/RTSPlayerController.h"
#include "../Player/PlayerPawn.h"
#include "Save/SaveManager.h"

const bool AColonyBuilderGameModeBase::SaveOnExit(true);

AColonyBuilderGameModeBase::AColonyBuilderGameModeBase()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
	PlayerControllerClass = ARTSPlayerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
}

void AColonyBuilderGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AColonyBuilderGameModeBase::SaveOnExit)
	{
		if (USaveManager* SaveManager = GetManager<USaveManager>(this))
		{
			//SaveManager->SaveGame();
		}
	}
}

void AColonyBuilderGameModeBase::BeginPlay()
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (UVillagerManager* VillagerManager = GetManager<UVillagerManager>(this))
		{
			VillagerManager->SetBaseVillagerClass(VillagerClass);
		}
	}
}

