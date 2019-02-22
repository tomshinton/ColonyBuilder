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

	UColonyInstance();

public:
	virtual void Init() override;
	UColonyManager* GetManager(TSubclassOf<UColonyManager> Manager);

private:

	UFUNCTION()
	void StartManager();

	UPROPERTY()
	TMap<TSubclassOf<UColonyManager>, UColonyManager*> Managers;
	TArray<TSubclassOf<UColonyManager>> ManagerClasses;
};
