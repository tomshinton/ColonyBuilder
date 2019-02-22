// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BaseVillager.h"

#include "JournalUtils.generated.h"

struct FJournal;
struct FJournalKey;
class ABaseVillager;

//////////////////////////////////////////////////////////////////////////
// Used for pushing info onto a journal and reading from it, as well as invalidating keys
//////////////////////////////////////////////////////////////////////////

UCLASS()
class UJournalUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Journal | Getters")
	static FVector GetKeyAsVector(FJournal InJournal, const FName InKeyName) { return GetKey(InJournal, InKeyName, EKeyType::Vector).GetVector(); };

	UFUNCTION(BlueprintCallable, Category = "Journal | Getters")
	static float GetKeyAsFloat(FJournal InJournal, const FName InKeyName) { return GetKey(InJournal, InKeyName, EKeyType::Float).GetFloat(); };

	UFUNCTION(BlueprintCallable, Category = "Journal | Getters")
	static FGuid GetKeyAsGuid(FJournal InJournal, const FName InKeyName) { return GetKey(InJournal, InKeyName, EKeyType::Guid).GetGuid(); };

	UFUNCTION(BlueprintCallable, Category = "Journal | Setters")
	static void SetKeyAsVector(ABaseVillager* InVillager, const FName InKeyName, const FVector InVector) { PushKey(GetVillagerJournal(InVillager), FJournalKey(InVector, InKeyName)); };

	UFUNCTION(BlueprintCallable, Category = "Journal | Setters")
	static void SetKeyAsFloat(ABaseVillager* InVillager, const FName InKeyName, const float InFloat) { PushKey(GetVillagerJournal(InVillager), FJournalKey(InFloat, InKeyName)); };

	UFUNCTION(BlueprintCallable, Category = "Journal | Setters")
	static void SetKeyAsGuid(ABaseVillager* InVillager, const FName InKeyName, const FGuid InGuid) { PushKey(GetVillagerJournal(InVillager), FJournalKey(InGuid, InKeyName)); };

	static FJournal& GetVillagerJournal(ABaseVillager* InVillager);
	static void PushKey(FJournal& InJournal, const FJournalKey InKey);
	static FJournalKey GetKey(FJournal& InJournal, const FName InName, const EKeyType InKeyType);

	static bool KeyExists(FJournal& InJournal, const FName InName, const EKeyType InKeyType);
};

