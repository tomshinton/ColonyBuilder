// ColonyBuilder Project, personal project by Tom Shinton

#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "ColonySave.h"
#include "SavableInterface.h"
#include "ColonyManager.h"
#include "Engine/World.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "PlayerPawn.h"

const FString USaveManager::SaveSlot(TEXT("Dev Slot"));

DEFINE_LOG_CATEGORY(SaveManager);

USaveManager::USaveManager()
{
	AutosaveFrequency = 5.f;
	AutosaveEnabled = true;
}

void USaveManager::PostInitProperties()
{
	Super::PostInitProperties();

	if (!CurrentSave) 
	{
		if (!UGameplayStatics::DoesSaveGameExist(SaveSlot, 0))
		{
			CurrentSave = Cast<UColonySave>(UGameplayStatics::CreateSaveGameObject(UColonySave::StaticClass()));
		}
		else
		{
			CurrentSave = Cast<UColonySave>(UGameplayStatics::LoadGameFromSlot(USaveManager::SaveSlot, 0));
		}
	}

	LoadGame(CurrentSave);
	StartAutosaveTimer();
}

void USaveManager::SaveGame()
{
	TArray<AActor*> FoundActors;
	TArray<FBuildingSaveData> SavedBuildings;

	if (UWorld* World = GetWorld())
	{

#pragma region Buildings
		UGameplayStatics::GetAllActorsWithInterface(World, USavableInterface::StaticClass(), FoundActors);

		CurrentSave->SavedBuildables.Empty();
		for (AActor* FoundActor : FoundActors)
		{
			ISavableInterface* SaveInterface = Cast<ISavableInterface>(FoundActor);
			CurrentSave->SavedBuildables.Add(SaveInterface->GetBuildingSaveData());
		}
#pragma endregion 
#pragma region Player
		if (!LocalPawnRef)
		{
			if (APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
			{
				if (APlayerPawn* PlayerPawn = Cast<APlayerPawn>(LocalPawn))
				{
					LocalPawnRef = PlayerPawn;
				}
			}
		}

		CurrentSave->PlayerSaveData = LocalPawnRef->GetSaveData();
#pragma endregion 

		UGameplayStatics::SaveGameToSlot(CurrentSave, USaveManager::SaveSlot, 0);
	}
}

void USaveManager::StartAutosaveTimer()
{
	if (GetWorld() && AutosaveEnabled)
	{
		AutosaveHandle.Invalidate();
		GetWorld()->GetTimerManager().SetTimer(AutosaveHandle, this, &USaveManager::SaveGame, AutosaveFrequency, true, 0.f);
	}
}

void USaveManager::SetAutosaveFrequency(int32 InAutosaveFrequency)
{
	AutosaveFrequency = InAutosaveFrequency;
	StartAutosaveTimer();
}

void USaveManager::LoadGame(UColonySave* SaveToLoad)
{
	if (SaveToLoad && GetWorld())
	{
#pragma region Buildings
		FActorSpawnParameters SpawnParams;

		for (FBuildingSaveData SavedBuilding : SaveToLoad->SavedBuildables)
		{
			if (auto* NewBuilding = GetWorld()->SpawnActor<AActor>(SavedBuilding.BuildingClass, SavedBuilding.BuildingTransform, SpawnParams))
			{
				ISavableInterface* SaveInterface = Cast<ISavableInterface>(NewBuilding);
				SaveInterface->LoadBuildingSaveData(SavedBuilding);
			}
		}
#pragma endregion
	}
}

FPlayerSaveData USaveManager::GetPlayerSaveInfo()
{
	return CurrentSave->PlayerSaveData;
}

