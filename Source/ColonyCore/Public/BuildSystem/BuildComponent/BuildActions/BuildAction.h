// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "Utils/DataTypes/BuildingDataTypes.h"
#include "Core/Player/RTSPlayerController.h"
#include "GhostBase.h"

#include "BuildAction.generated.h"

class UBuildingData;

//////////////////////////////////////////////////////////////////////////
// Base class for all Build actions
// - Fire and Forget
// - Linear
// - Grid
//////////////////////////////////////////////////////////////////////////

UCLASS(abstract)
class COLONYCORE_API UBuildAction : public UObject
{
	GENERATED_BODY()

public:

	virtual void OnBuild() {};
	virtual void RotatePlacement() {};

	virtual void StartAction() { IsBuilding = true; };
	virtual void CompleteAction() { IsBuilding = false; };

	virtual void SpawnGhost()
	{
		if (!SpawnedGhost)
		{
			if (UWorld* World = GetWorld())
			{
				FActorSpawnParameters GhostSpawnParams;
				SpawnedGhost = World->SpawnActor<AGhost>(RoundedLocUnderMouse, FRotator(0.f, 0.f, 0.f), GhostSpawnParams);
				if (SpawnedGhost)
				{
					SpawnedGhost->SetGhostInfo(BuildingData);
				}
			}
		}
	};

	virtual void CancelAction()
	{
		if (IsBuilding)
		{
			IsBuilding = false;
		}
		else
		{
			if (SpawnedGhost)
			{
				SpawnedGhost->Destroy();
			}

			ConditionalBeginDestroy();
		}
	};
	
public:

	void SetBuildingData(UBuildingData* InBuildingData) { BuildingData = InBuildingData; };
	virtual class UWorld* GetWorld() const override
	{
		Super::GetWorld();

		if (GetOuter())
		{
			return GetOuter()->GetWorld();
		}

		return nullptr;
	}
	virtual void PostInitProperties() override
	{
		Super::PostInitProperties();

		if (UWorld* World = GetWorld())
		{
			if (ARTSPlayerController* LocalController = Cast<ARTSPlayerController>(World->GetFirstPlayerController()))
			{
				LocalController->OnMouseMoved.AddDynamic(this, &UBuildAction::UpdateMouseCoords);
				CachedController = LocalController;
			}
		}
	}

	UFUNCTION()
	void UpdateMouseCoords(const FVector& InLocUnderMouse, const FVector& InRoundedLocUnderMouse)
	{
		LocUnderMouse = InLocUnderMouse;
		RoundedLocUnderMouse = InRoundedLocUnderMouse;
	}

protected:
	
	UPROPERTY()
	AGhost* SpawnedGhost = nullptr;

	UPROPERTY()
	UBuildingData* BuildingData = nullptr;

	UPROPERTY()
	ARTSPlayerController* CachedController = nullptr;

	FVector LocUnderMouse;
	FVector RoundedLocUnderMouse;

	bool IsBuilding = false;
};