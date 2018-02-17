// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildComponent.h"

#include "BuildingData.h"

#include "RTSPlayerController.h"
#include "PlayerPawn.h"
#include "Ghost.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SpawnedGhost = nullptr;
}

void UBuildComponent::BeginPlay()
{
	ControllerRef = Cast<ARTSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void UBuildComponent::SetEnabled(bool InEnabled)
{
	Super::SetEnabled(InEnabled);

	if (IsEnabled)
	{
		if (ControllerRef && !ControllerRef->OnMouseMoved.IsBound())
		{
			ControllerRef->OnMouseMoved.AddDynamic(this, &UBuildComponent::UpdateMouseCoords);
		}

		OwningPawn->OnConfirmAction.BindUObject(this, &UBuildComponent::ConfirmPlacement);
		OwningPawn->OnCancelAction.BindUObject(this, &UBuildComponent::CancelBuild);
		OwningPawn->OnRotatePlacement.BindUObject(this, &UBuildComponent::RotatePlacement);
	}
	else
	{
		if (ControllerRef && ControllerRef->OnMouseMoved.IsBound()) //General cleanup, dont want this running if its not needed
		{
			ControllerRef->OnMouseMoved.RemoveDynamic(this, &UBuildComponent::UpdateMouseCoords);

			if (SpawnedGhost)
			{
				SpawnedGhost->Destroy();
				SpawnedGhost = nullptr;
			}
		}

		OwningPawn->OnConfirmAction.Unbind();
		OwningPawn->OnCancelAction.Unbind();
		OwningPawn->OnRotatePlacement.Unbind();
	}
}

void UBuildComponent::UpdateMouseCoords(FVector InCurrMouseCoords, FVector InRoundedMouseCoords)
{
	CurrMouseCoords = InCurrMouseCoords;
	CurrRoundedMouseCoords = InRoundedMouseCoords;

	UpdateGhostLocation();
}

void UBuildComponent::RotatePlacement()
{
	if (SpawnedGhost)
	{
		FRotator NewRotation = SpawnedGhost->GetActorRotation();
		NewRotation.Yaw += RotationRate;
		SpawnedGhost->SetActorRotation(NewRotation);
	}
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
		SpawnedGhost = GetWorld()->SpawnActor<AGhost>(CurrRoundedMouseCoords, FRotator::ZeroRotator, SpawnParams);
		SpawnedGhost->SetGhostInfo(CurrentBuildingData);
	}
	else
	{
		CancelBuild();
	}
}

void UBuildComponent::ConfirmPlacement()
{
	FVector CurrentGhostLoc;
	FRotator CurrentGhostRot;

	if (SpawnedGhost && SpawnedGhost->GetIsValid())
	{
		CurrentGhostLoc = SpawnedGhost->GetActorLocation();
		CurrentGhostRot = SpawnedGhost->GetActorRotation();
		SpawnedGhost->Destroy();
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<ABuildableBase>(CurrentBuildingData->BuildingClass, CurrentGhostLoc, CurrentGhostRot);

		SetEnabled(false);
	}
}

void UBuildComponent::CancelBuild()
{	
	SetEnabled(false);
}

void UBuildComponent::UpdateGhostLocation()
{
	if (SpawnedGhost)
	{
		SpawnedGhost->SetActorLocation(CurrRoundedMouseCoords);
	}
}

