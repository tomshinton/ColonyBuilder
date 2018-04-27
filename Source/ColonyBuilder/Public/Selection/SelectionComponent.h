// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/RTSBaseComp.h"
#include "SelectionInterface.h"
#include "SelectionComponent.generated.h"

class APlayerPawn;
class ARTSPlayerController;

UCLASS()
class COLONYBUILDER_API USelectionComponent : public URTSBaseComp
{
	GENERATED_BODY()

public:
	virtual void SetEnabled(bool InEnabled) override;

	UFUNCTION()
	void HoverCheck(const FVector& PosUnderMouse, const FVector& PosUnderMouseRounded);

	UFUNCTION()
	void Select();

	UFUNCTION()
	void CancelSelection();

private:

	ISelectionInterface* HoveredInterface;
	ISelectionInterface* SelectedInterface;
};
