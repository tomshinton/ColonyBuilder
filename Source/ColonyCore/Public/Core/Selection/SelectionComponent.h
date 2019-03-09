// ColonyBuilder Project, personal project by Tom Shinton

#pragma once


#include "../Components/RTSBaseComp.h"
#include "SelectionComponent.generated.h"

class ISelectionInterface;
class APlayerPawn;
class ARTSPlayerController;

UCLASS()
class USelectionComponent : public URTSBaseComp
{
	GENERATED_BODY()

public:

	virtual void PostInitProperties() override;

	UFUNCTION()
	void HoverCheck(const FVector& PosUnderMouse, const FVector& PosUnderMouseRounded);

	UFUNCTION()
	void Select();

	UFUNCTION()
	void CancelSelection();

private:

	TScriptInterface<ISelectionInterface> HoveredInterface;
	TScriptInterface<ISelectionInterface> SelectedInterface;
};
