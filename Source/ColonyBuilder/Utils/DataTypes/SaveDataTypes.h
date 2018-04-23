// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FBuildingSaveData
{
	GENERATED_USTRUCT_BODY()

	FBuildingSaveData() {};

	FBuildingSaveData(TSubclassOf<AActor> InBuildingClass, FTransform InTransform, UStaticMesh* InMesh) :
		BuildingClass(InBuildingClass)
		, BuildingTransform(InTransform)
		, BuildingMesh(InMesh)
	{};

public:
	UPROPERTY()
	TSubclassOf<AActor> BuildingClass;

	UPROPERTY()
	FTransform BuildingTransform;

	UPROPERTY()
	UStaticMesh* BuildingMesh;
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