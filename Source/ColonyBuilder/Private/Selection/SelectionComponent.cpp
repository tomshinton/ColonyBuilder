// ColonyBuilder Project, personal project by Tom Shinton

#include "SelectionComponent.h"

#include "PlayerPawn.h"
#include "RTSPlayerController.h"
#include "Engine/EngineTypes.h"
#include "Utils/Libraries/CollisionChannels.h"

void USelectionComponent::SetEnabled(bool InEnabled)
{
	Super::SetEnabled(InEnabled);

	if (InEnabled)
	{
		OwningController->OnMouseMoved.AddDynamic(this, &USelectionComponent::HoverCheck);

		if (!OwningPawn->OnEndConfirmAction.IsBound())
		{
			OwningPawn->OnEndConfirmAction.AddDynamic(this, &USelectionComponent::Select);
		}

		if (!OwningPawn->OnCancelAction.IsBound())
		{
			OwningPawn->OnCancelAction.AddDynamic(this, &USelectionComponent::CancelSelection);
		}
	}
	else
	{
		OwningController->OnMouseMoved.RemoveDynamic(this, &USelectionComponent::HoverCheck);
	}
}

void USelectionComponent::HoverCheck(const FVector& PosUnderMouse, const FVector& PosUnderMouseRounded)
{
	if (GetWorld())
	{
	
		FHitResult HitResult;
		OwningController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(CC_SELECTION), true, HitResult);

		if (!HitResult.Actor.Get() && HoveredInterface)
		{
			HoveredInterface->OnEndHover();
		}

		if (ISelectionInterface* SelectionInterface = Cast<ISelectionInterface>(HitResult.Actor.Get()))
		{
			if (SelectionInterface != HoveredInterface)
			{
				SelectionInterface->OnReceiveHover();
				if (HoveredInterface)
				{
					HoveredInterface->OnEndHover();
				}
				HoveredInterface = SelectionInterface;
			}
		}
		else
		{
			if (HoveredInterface)
			{
				HoveredInterface->OnEndHover();
				HoveredInterface = nullptr;
			}
		}
	}
}

void USelectionComponent::Select()
{
	if (!HoveredInterface || HoveredInterface != SelectedInterface)
	{
		CancelSelection();
	}

	if (HoveredInterface)
	{
		HoveredInterface->OnSelect();
		SelectedInterface = HoveredInterface;
	}

}

void USelectionComponent::CancelSelection()
{
	if (SelectedInterface)
	{
		SelectedInterface->OnEndSelect();
	}
}

