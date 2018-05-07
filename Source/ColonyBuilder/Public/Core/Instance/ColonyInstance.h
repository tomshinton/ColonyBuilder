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

	UFUNCTION(BlueprintCallable, Category = "Managers")
	UColonyManager* GetManagerByClass(TSubclassOf<UColonyManager> ManagerClass);

private:

	UPROPERTY()
	TArray<UColonyManager*> Managers;

};
