// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingDataTypes.h"
#include "Professions.h"

#include "SaveDataTypes.generated.h"

class UBuildingData;
class APawn;

//////////////////////////////////////////////////////////////////////////
// All structs to be serialised by the SaveManager
//////////////////////////////////////////////////////////////////////////

//The state of ongoing construction projects, such as percentage done
USTRUCT()
struct FConstructionSaveData
{
	GENERATED_USTRUCT_BODY()

	FConstructionSaveData() {};

	FConstructionSaveData(EConstructionStage InCurrentStage, float InRemainingBuildTime, float InTotalBuildTime) :
		CurrentStage(InCurrentStage)
		, RemainingBuildTime(InRemainingBuildTime)
		, TotalBuildTime(InTotalBuildTime)
	{}

public:

	UPROPERTY()
	EConstructionStage CurrentStage;

	UPROPERTY()
	float RemainingBuildTime;

	UPROPERTY()
	float TotalBuildTime;
};

//Saving persistant buildings that are placed in the world
USTRUCT(BlueprintType)
struct FBuildingSaveData
{
	GENERATED_USTRUCT_BODY()

	FBuildingSaveData() {};

	FBuildingSaveData(FGuid InID, TSubclassOf<AActor> InBuildingClass, FTransform InTransform, UStaticMesh* InMesh) :
		BuildingClass(InBuildingClass)
		, BuildingTransform(InTransform)
		, BuildingMesh(InMesh)
	{};

	FBuildingSaveData(const FGuid InID, const TSubclassOf<AActor> InBuildingClass, UBuildingData* InBuildingData, const FTransform InTransform, UStaticMesh* InMesh, const FConstructionSaveData InConstructionData, const TArray<FGuid> InRegisteredEmployees, const TArray<FGuid> InRegisteredResidents) :
		  ID(InID)
		, BuildingClass(InBuildingClass)
		, BuildingData(InBuildingData)
		, BuildingTransform(InTransform)
		, BuildingMesh(InMesh)
		, ConstructionData(InConstructionData)
		, RegisteredEmployees(InRegisteredEmployees)
		, RegisteredResidents(InRegisteredResidents)
	{};

public:

	UPROPERTY()
	FGuid ID;

	UPROPERTY()
	TSubclassOf<AActor> BuildingClass;

	UPROPERTY()
	UBuildingData* BuildingData;

	UPROPERTY()
	FTransform BuildingTransform;

	UPROPERTY()
	UStaticMesh* BuildingMesh;

	UPROPERTY()
	FConstructionSaveData ConstructionData;

	UPROPERTY()
	TArray<FGuid> RegisteredEmployees;

	UPROPERTY()
	TArray<FGuid> RegisteredResidents;
};

//Info about the player, such as pawn location and camera rotation/zoom
USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_USTRUCT_BODY()

	FPlayerSaveData() :
	IsValidSetting(false)
	{};

	FPlayerSaveData(bool InIsValidSetting, FTransform InPlayerTransform, FTransform InCameraTransform) :
		IsValidSetting(InIsValidSetting)
		, PlayerTransform(InPlayerTransform)
		, CameraTransform(InCameraTransform)
	{};

public:

	UPROPERTY()
	bool IsValidSetting;

	UPROPERTY()
	FTransform PlayerTransform;

	UPROPERTY()
	FTransform CameraTransform;
};

//Important Guids for Villagers, such as where they live and where they work
USTRUCT(BlueprintType)
struct FVillagerLocationData
{
	GENERATED_BODY();

	FVillagerLocationData()
		: ResidenceID()
		, WorkplaceID()
	{}

	FVillagerLocationData(const FGuid InResidenceID, const FGuid InWorkplaceID)
		: ResidenceID(InResidenceID)
		, WorkplaceID(InWorkplaceID)
	{}

	UPROPERTY()
		FGuid ResidenceID;

	UPROPERTY()
		FGuid WorkplaceID;
};

//Saving info about each villager
USTRUCT(BlueprintType)
struct FVillagerSaveData
{
	GENERATED_BODY();

	FVillagerSaveData() 
		: VillagerID()
		, PawnClass()
		, VillagerTransform(FTransform())
		, ImportantLocations()
		, Profession(nullptr)
	{}


	FVillagerSaveData(FGuid InVillagerID, TSubclassOf<APawn> InPawnClass, const FTransform InTransform, const FVillagerLocationData InImportantLocations, const TSubclassOf<UProfessionBase> InProfession)
		: VillagerID(InVillagerID)
		, PawnClass(InPawnClass)
		, VillagerTransform(InTransform)
		, ImportantLocations(InImportantLocations)
		, Profession(InProfession)
	{}

	UPROPERTY()
	FGuid VillagerID;

	UPROPERTY()
	TSubclassOf<APawn> PawnClass;

	UPROPERTY()
	FTransform VillagerTransform;

	UPROPERTY()
	FVillagerLocationData ImportantLocations;

	UPROPERTY()
	TSubclassOf<UProfessionBase> Profession;
};

