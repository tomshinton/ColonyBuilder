// ColonyBuilder Project, personal project by Tom Shinton

#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "ColonySave.h"
#include "SavableInterface.h"
#include "ColonyManager.h"
#include "Engine/World.h"
#include "Utils/DataTypes/SaveDataTypes.h"

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
			CurrentSave = Cast<UColonySave>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));
			LoadGame(CurrentSave);
		}
	}

	StartAutosaveTimer();
}

void USaveManager::SaveGame()
{
	TArray<AActor*> FoundActors;
	TArray<FSaveData> RetrievedInfo;

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsWithInterface(World, USavableInterface::StaticClass(), FoundActors);
	
		for (AActor* FoundActor : FoundActors)
		{
			ISavableInterface* SaveInterface = Cast<ISavableInterface>(FoundActor);
			RetrievedInfo.Add(SaveInterface->GetSaveData());
		}
	
		CurrentSave->SavedActors = RetrievedInfo;

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
	if (SaveToLoad)
	{
		FActorSpawnParameters SpawnParams;

		for (FSaveData SavedActor : SaveToLoad->SavedActors)
		{
			//FBuildingSaveData* BuildingData = Cast<FBuildingSaveData>(&SavedActor);
			FBuildingSaveData* BuildingData = reinterpret_cast<FBuildingSaveData*>(&SavedActor);
			FBuildingSaveData BuildingData = (FBuildingSaveData*)&SavedActor;
			GetWorld()->SpawnActor<AActor>(BuildingData->BuildingClass, BuildingData->BuildingTransform, SpawnParams);
		}
	}
}

