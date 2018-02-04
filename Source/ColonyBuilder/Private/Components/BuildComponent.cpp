// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildComponent.h"

#include "BuildingData.h"

#include "RTSPlayerController.h"
#include "Ghost.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UBuildComponent::BeginPlay()
{
	Cast<ARTSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->OnMouseMoved.AddDynamic(this, &UBuildComponent::UpdateMouseCoords);
}

void UBuildComponent::UpdateMouseCoords(FVector InCurrMouseCoords, FVector InRoundedMouseCoords)
{
	CurrMouseCoords = InCurrMouseCoords;
	CurrRoundedMouseCoords = InRoundedMouseCoords;

	UpdateGhostTrans();
}

void UBuildComponent::StartBuildingFromClass(UBuildingData* BuildingData)
{
	SetEnabled(true);
	CurrentBuildingData = BuildingData;

	if (CurrentBuildingData)
	{
		if (SpawnedGhost)
		{
			SpawnedGhost->Destroy();
		}
		FActorSpawnParameters SpawnParams;
		//SpawnedGhost = GetWorld()->SpawnActor(AGhost::StaticClass(), CurrRoundedMouseCoords, FRotator::ZeroRotator, SpawnParams);
		SpawnedGhost = GetWorld()->SpawnActor<AGhost>(CurrRoundedMouseCoords, FRotator::ZeroRotator, SpawnParams);
		SpawnedGhost->SetGhost(CurrentBuildingData->GhostMesh);
	}
	else
	{
		CancelBuild();
	}
}

void UBuildComponent::CancelBuild()
{

	if (SpawnedGhost)
	{
		SpawnedGhost->Destroy();
	}

	SetEnabled(false);
}

void UBuildComponent::UpdateGhostTrans()
{
	if (SpawnedGhost)
	{
		SpawnedGhost->SetActorLocation(CurrRoundedMouseCoords);
	}
}

