// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildActionFF.h"
#include "Utils/DataTypes/BuildingDataTypes.h"

DEFINE_LOG_CATEGORY(BuildActionFFLog);

void UBuildActionFF::OnBuild()
{
	Super::OnBuild();

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
	if (SpawnedGhost)
	{
		const FRotator CurrentRotation = SpawnedGhost->GetActorRotation();
		SpawnedGhost->SetActorRotation(CurrentRotation + FRotator(0.f, 45.f, 0.f));
	}
}

void UBuildActionFF::SpawnGhost()
{
	Super::SpawnGhost();
}

void UBuildActionFF::UpdateGhost(const FVector& InRoundedLocUnderMouse)
{
	TArray<FSubBuilding> SubBuildings;
	SpawnedGhost->UpdateGhost(InRoundedLocUnderMouse, SubBuildings);
}
