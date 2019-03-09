// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class UColonyManager;
class APlayerPawn;

#include "Utils/DataTypes/SaveDataTypes.h"
#include "SaveManager.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(SaveManager, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveOver, bool, WasSaveSuccessful);

UCLASS(Blueprintable)
class USaveManager : public UColonyManager
{
	GENERATED_BODY()

	USaveManager();

	static const FString SaveSlot;

	virtual void Init(const TFunction<void() > InitCallback) override;

	UFUNCTION(BlueprintCallable, Category = "Auto Save")
	void SetAutosaveFrequency(int32 InAutosaveFrequency);
	UFUNCTION(BlueprintPure, Category = "Auto Save")
	int32 GetAutosaveFrequency() { return AutosaveFrequency; }

	UFUNCTION(BlueprintCallable, Category = "Auto Save")
	void SetAutosaveEnabled(bool InAutosaveEnabled) { AutosaveEnabled = InAutosaveEnabled; }
	UFUNCTION(BlueprintPure, Category = "Auto Save")
	bool GetAutosaveEnabled() { return AutosaveEnabled; }
	
	
public:
	UFUNCTION(BlueprintPure, Category = "Save Game")
	FPlayerSaveData GetCachedPlayerData() { return CachedPlayerData; }

	int32 AutosaveFrequency;

	UPROPERTY(BlueprintAssignable, Category = "Save Game")
	FOnSaveStarted OnSaveStarted;

	UPROPERTY(BlueprintAssignable, Category = "Save Game")
	FOnSaveOver OnSaveOver;

	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Load Game")
	void LoadGame(UColonySave* SaveToLoad);

	FPlayerSaveData GetPlayerSaveInfo();

private:

	void StartAutosaveTimer();

	FTimerHandle AutosaveHandle;
	bool AutosaveEnabled;

	FPlayerSaveData CachedPlayerData;

	UPROPERTY()
	TWeakObjectPtr<APlayerPawn> LocalPawnRef;
};
