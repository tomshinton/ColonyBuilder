// ColonyBuilder Project, personal project by Tom Shinton

#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "ColonySave.h"
#include "SavableInterface.h"
#include "ColonyManager.h"
#include "Engine/World.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "PlayerPawn.h"

#include "AI/Navigation/NavigationSystem.h"

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

	if (UColonySave* CurrentSave = Cast<UColonySave>(UGameplayStatics::LoadGameFromSlot(USaveManager::SaveSlot, 0)))
	{
		LoadGame(CurrentSave);
	}

	if (AutosaveEnabled)
	{
		StartAutosaveTimer();
	}
}

void USaveManager::SaveGame()
{
	UE_LOG(SaveManager, Log, TEXT("Saving Game"));
	TArray<AActor*> FoundActors;
	TArray<FBuildingSaveData> SavedBuildings;

	if (UWorld* World = GetWorld())
	{
		UColonySave* CurrentSave = Cast<UColonySave>(UGameplayStatics::CreateSaveGameObject(UColonySave::StaticClass()));

		//Get/Create a new save
		if (CurrentSave)
		{
#pragma region Buildings
			UE_LOG(SaveManager, Log, TEXT("Saving Buildings"));
			UGameplayStatics::GetAllActorsWithInterface(World, USavableInterface::StaticClass(), FoundActors);

			CurrentSave->SavedBuildables.Empty();
			for (AActor* FoundActor : FoundActors)
			{
				if (ISavableInterface* SaveInterface = Cast<ISavableInterface>(FoundActor))
				{
					CurrentSave->SavedBuildables.Add(SaveInterface->GetBuildingSaveData());
				}
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
			UE_LOG(SaveManager, Log, TEXT("Saving Player Info"));
			CurrentSave->PlayerSaveData = LocalPawnRef->GetSaveData();
#pragma endregion 
			UE_LOG(SaveManager, Log, TEXT("Saving data to file"));
			UGameplayStatics::SaveGameToSlot(CurrentSave, USaveManager::SaveSlot, 0);
		}
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
#pragma region Player
		CachedPlayerData = SaveToLoad->PlayerSaveData;
#pragma endregion Player
	}
}

