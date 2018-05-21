// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuildingDataTypes.h"

#include "SaveDataTypes.generated.h"

class UBuildingData;

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

USTRUCT(BlueprintType)
struct FBuildingSaveData
{
	GENERATED_USTRUCT_BODY()

	FBuildingSaveData() {};

	FBuildingSaveData(TSubclassOf<AActor> InBuildingClass, FTransform InTransform, UStaticMesh* InMesh, int32 InBuildingID) :
		BuildingClass(InBuildingClass)
		, BuildingTransform(InTransform)
		, BuildingMesh(InMesh)
		, BuildingID(InBuildingID)
	{};

	FBuildingSaveData(TSubclassOf<AActor> InBuildingClass, UBuildingData* InBuildingData, FTransform InTransform, UStaticMesh* InMesh, FConstructionSaveData InConstructionData, int32 InBuildingID) :
		BuildingClass(InBuildingClass)
		, BuildingData(InBuildingData)
		, BuildingTransform(InTransform)
		, BuildingMesh(InMesh)
		, ConstructionData(InConstructionData)
		, BuildingID(InBuildingID)
	{};

public:
	UPROPERTY()
	TSubclassOf<AActor> BuildingClass;

	UPROPERTY()
	UBuildingData* BuildingData;

	UPROPERTY()
	FTransform BuildingTransform;

	UPROPERTY()
	UStaticMesh* BuildingMesh;

	UPROPERTY()
	float BuildingID;

	UPROPERTY()
	FConstructionSaveData ConstructionData;
};

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