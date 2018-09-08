// Fill out your copyright notice in the Description page of Project Settings.

#include "ColonyBuilderGameModeBase.h"
#include "RTSPlayerController.h"

#include "HUD/RTSHUD.h"

#include "ColonyInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveManager.h"
#include "VillagerManager.h"

const int32 AColonyBuilderGameModeBase::GridSize(256);
const bool AColonyBuilderGameModeBase::SaveOnExit(true);

AColonyBuilderGameModeBase::AColonyBuilderGameModeBase()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
	PlayerControllerClass = ARTSPlayerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
}

void AColonyBuilderGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(AColonyBuilderGameModeBase::SaveOnExit)
	{
		if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			if(USaveManager* SaveManager = GameInst->GetManager<USaveManager>())
			{
				//SaveManager->SaveGame();
			}
		}
	}
}

void AColonyBuilderGameModeBase::BeginPlay()
{
	if (UColonyInstance* GameInst = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (UVillagerManager* VillagerManager = GameInst->GetManager<UVillagerManager>())
		{
			VillagerManager->SetBaseVillagerClass(VillagerClass);
		}
	}
}

