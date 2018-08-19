// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Utils/DataTypes/SaveDataTypes.h"

#include "BaseVillager.generated.h"

class AAIController;

UCLASS()
class COLONYAI_API ABaseVillager : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseVillager();

	FVillagerSaveData GetSaveData();
	void LoadVillagerSaveData(const FVillagerSaveData& InData);
};
