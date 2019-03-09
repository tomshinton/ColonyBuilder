// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildActionFF.h"
#include "Utils/DataTypes/BuildingDataTypes.h"

DEFINE_LOG_CATEGORY(BuildActionFFLog);

void UBuildActionFF::OnBuild()
{
	if (UWorld* World = GetWorld())
	{
		SpawnGhost();
	}
}

void UBuildActionFF::StartAction()
{
	Super::StartAction();
}

void UBuildActionFF::CompleteAction()
{
	Super::CompleteAction();
}

void UBuildActionFF::CancelAction()
{	
	Super::CancelAction();
}

void UBuildActionFF::RotatePlacement()
{

}

void UBuildActionFF::SpawnGhost()
{
	Super::SpawnGhost();

	if (SpawnedGhost)
	{
		if (ARTSPlayerController* LocalController = Cast<ARTSPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			LocalController->OnMouseMoved.AddDynamic(this, &UBuildActionFF::UpdateGhost);
		}
	}
}

void UBuildActionFF::UpdateGhost(const FVector& InLocUnderMouse, const FVector& InRoundedLocUnderMouse)
{
	TArray<FSubBuilding> SubBuildings;
	SpawnedGhost->UpdateGhost(InRoundedLocUnderMouse, SubBuildings);
}
