// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ColonyManager.h"
#include "ColonyInstance.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ColonyInstanceLog, Log, All);

UCLASS()
class COLONYBUILDER_API UColonyInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Managers")
	void StartManager(TSubclassOf<UColonyManager> ManagerClass, FString ManagerName);

	/*best BP implementation*/
	UFUNCTION(BlueprintCallable, Category = "Managers")
	UColonyManager* GetManagerFromClass(TSubclassOf<UColonyManager> InManagerClass);

	TArray<UColonyManager*> GetManagers() { return Managers; }

#pragma region Templates

#pragma endregion

private:

	UPROPERTY()
	TArray<UColonyManager*> Managers;
};
