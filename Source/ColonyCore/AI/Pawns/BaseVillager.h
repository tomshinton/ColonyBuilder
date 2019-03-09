// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "Utils/DataTypes/SaveDataTypes.h"
#include "Core/Selection/SelectionInterface.h"
#include "AI/Journal/Journal.h"

#include "BaseVillager.generated.h"

class UPlan;
class UCapsuleComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;

//////////////////////////////////////////////////////////////////////////
// Base class for all Villagers in the colony
//////////////////////////////////////////////////////////////////////////

DECLARE_LOG_CATEGORY_EXTERN(VillagerLog, All, All)

UCLASS()
class COLONYCORE_API ABaseVillager : public AActor, public ISelectionInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Collision")
	UCapsuleComponent* Capsule;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Collision")
	USkeletalMeshComponent* SkelMeshComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Collision")
	UStaticMeshComponent* StaticMeshComponent;

	UFUNCTION(BlueprintPure, Category = "AI")
	UPlan* GetPlan() const { return Plan; };

	UFUNCTION(BlueprintPure, Category = "AI")
	FJournal& GetJournal() { return Journal; }

	virtual void BeginPlay() override;

	static const float AgentRadius;
	static const float AgentHeight;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
	float MoveSpeed;

public:
	ABaseVillager();

	FVillagerSaveData GetSaveData();
	void LoadVillagerSaveData(const FVillagerSaveData& InData);

	TSubclassOf<UProfessionBase> GetProfession() { return Profession; }
	void SetProfession(const TSubclassOf<UProfessionBase> InNewProfession) { Profession = InNewProfession; };

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

	UPROPERTY()
	UPlan* Plan;

	UPROPERTY()
	FJournal Journal;
};
