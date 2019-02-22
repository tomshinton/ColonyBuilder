// ColonyBuilder Project, personal project by Tom Shinton

#include "SelectionComponent.h"

#include "PlayerPawn.h"
#include "RTSPlayerController.h"
#include "Engine/EngineTypes.h"
#include "Utils/Libraries/CollisionChannels.h"

void USelectionComponent::PostInitProperties()
{
	Super::PostInitProperties();

	if (UWorld* World = GetOuter()->GetWorld())
	{
		if (OwningController)
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
	}
}

void USelectionComponent::HoverCheck(const FVector& PosUnderMouse, const FVector& PosUnderMouseRounded)
{
	if (GetWorld())
	{
		FHitResult HitResult;
		OwningController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(CC_SELECTION), true, HitResult);

		if (!HitResult.Actor.Get() && HoveredInterface.GetInterface())
		{
			HoveredInterface->OnEndHover();
		}

		AActor* HitActor = HitResult.Actor.Get();

		if (ISelectionInterface* SelectionInterface = Cast<ISelectionInterface>(HitActor))
		{
			if (SelectionInterface != HoveredInterface.GetInterface())
			{
				SelectionInterface->OnReceiveHover();

				if (HoveredInterface.GetInterface())
				{
					HoveredInterface->OnEndHover();
				}

				HoveredInterface.SetInterface(SelectionInterface);
				HoveredInterface.SetObject(HitActor);
			}
		}
		else
		{
			if (HoveredInterface.GetObject())
			{
				HoveredInterface->OnEndHover();
				HoveredInterface = nullptr;
			}
		}
	}
}

void USelectionComponent::Select()
{
	if (!HoveredInterface.GetObject() || HoveredInterface != SelectedInterface)
	{
		CancelSelection();
	}

	if (HoveredInterface.GetObject())
	{
		HoveredInterface->OnSelect();
		SelectedInterface = HoveredInterface;
	}

}

void USelectionComponent::CancelSelection()
{
	if (SelectedInterface.GetObject())
	{
		SelectedInterface->OnEndSelect();

		SelectedInterface.SetObject(nullptr);
		SelectedInterface.SetInterface(nullptr);
	}
}


