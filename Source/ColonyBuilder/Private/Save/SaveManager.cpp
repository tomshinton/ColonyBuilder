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
#include "BaseVillager.h"
#include "VillagerManager.h"
#include "ColonyInstance.h"
#include "Utils/Libraries/ManagerUtils.h"

const FString USaveManager::SaveSlot(TEXT("Dev Slot"));

DEFINE_LOG_CATEGORY(SaveManager);

USaveManager::USaveManager()
{
	AutosaveFrequency = 5.f;
	AutosaveEnabled = false;
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
			if (!LocalPawnRef.IsValid())
			{
				if (APawn* LocalPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
				{
					if (APlayerPawn* PlayerPawn = Cast<APlayerPawn>(LocalPawn))
					{
						LocalPawnRef = PlayerPawn;
					}
				}
			}

			if (LocalPawnRef.IsValid())
			{
				UE_LOG(SaveManager, Log, TEXT("Saving Player Info"));
				CurrentSave->PlayerSaveData = LocalPawnRef->GetSaveData();
			}
#pragma endregion 

#pragma region AI

			TArray<AActor*> FoundVillagers;
			UGameplayStatics::GetAllActorsOfClass(World, ABaseVillager::StaticClass(), FoundVillagers);

			for (AActor* FoundVillager : FoundVillagers)
			{
				if (ABaseVillager* Villager = Cast<ABaseVillager>(FoundVillager))
				{
					CurrentSave->SavedVillagers.Add(Villager->GetSaveData());
				}
			}


#pragma endregion 

				UE_LOG(SaveManager, Log, TEXT("Saving data to file"));
				UGameplayStatics::SaveGameToSlot(CurrentSave, USaveManager::SaveSlot, 0);
		}
	}
}

void USaveManager::StartAutosaveTimer()
{
	if (UWorld* World = GetWorld())
	{
		if (AutosaveEnabled)
		{
			AutosaveHandle.Invalidate();
			World->GetTimerManager().SetTimer(AutosaveHandle, this, &USaveManager::SaveGame, AutosaveFrequency, true, 0.f);
		}
	}
}

void USaveManager::SetAutosaveFrequency(int32 InAutosaveFrequency)
{
	AutosaveFrequency = InAutosaveFrequency;
	StartAutosaveTimer();
}

void USaveManager::LoadGame(UColonySave* SaveToLoad)
{
	if (SaveToLoad)
	{
		if (UWorld* World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;

			for (FBuildingSaveData& SavedBuilding : SaveToLoad->SavedBuildables)
			{
				if (auto* NewBuilding = GetWorld()->SpawnActor<AActor>(SavedBuilding.BuildingClass, SavedBuilding.BuildingTransform, SpawnParams))
				{
					ISavableInterface* SaveInterface = Cast<ISavableInterface>(NewBuilding);
					SaveInterface->LoadBuildingSaveData(SavedBuilding);
				}
			}
		
			//Store player data because the Player hasnt spawned yet
			CachedPlayerData = SaveToLoad->PlayerSaveData;

			for (FVillagerSaveData& SavedVillagerData : SaveToLoad->SavedVillagers)
			{
				if (SavedVillagerData.PawnClass)
				{
					if (UVillagerManager* VillagerManager = GetManager<UVillagerManager>(World))
					{
						//Respawn the villagers from save
						VillagerManager->CreateVillagerFromSavedata(SavedVillagerData);
					}
				}
			}
		}
	}

}
