// ColonyBuilder Project, personal project by Tom Shinton

#include "SaveManager.h"
#include "Kismet/GameplayStatics.h"
#include "ColonySave.h"


DEFINE_LOG_CATEGORY(SaveManager);

USaveManager::USaveManager()
{
	AutosaveFrequency = 5.f;
	AutosaveEnabled = true;
}

void USaveManager::PostInitProperties()
{
	Super::PostInitProperties();

	StartAutosaveTimer();
}

void USaveManager::SaveGame()
{

	UColonySave* SaveGameInst = Cast<UColonySave>(UGameplayStatics::CreateSaveGameObject(UColonySave::StaticClass()));

	if (UGameplayStatics::SaveGameToSlot(SaveGameInst, SaveGameInst->SlotName, 0))
	{
		SaveGameInst->LogInfo();
		CurrentSave = SaveGameInst;
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
