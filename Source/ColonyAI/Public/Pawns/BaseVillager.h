// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Utils/DataTypes/SaveDataTypes.h"
#include "SelectionInterface.h"

#include "BaseVillager.generated.h"

//////////////////////////////////////////////////////////////////////////
// Base class for all Villagers in the colony
//////////////////////////////////////////////////////////////////////////

DECLARE_LOG_CATEGORY_EXTERN(VillagerLog, All, All)

class AAIController;

UCLASS()
class COLONYAI_API ABaseVillager : public ACharacter, public ISelectionInterface
{
	GENERATED_BODY()

public:
	ABaseVillager();

	FVillagerSaveData GetSaveData();
	void LoadVillagerSaveData(const FVillagerSaveData& InData);

	TSubclassOf<UProfessionBase> GetProfession() { return Profession; }

	FGuid VillagerID;
	FGuid ResidenceID;
	FGuid WorkplaceID;

	

//Selection Interface
	virtual void OnReceiveHover() override;
	virtual void OnEndHover() override;
	virtual void OnSelect() override;
	virtual void OnEndSelect() override;
//Selection Interface

private:
	TSubclassOf<UProfessionBase> Profession;
};
