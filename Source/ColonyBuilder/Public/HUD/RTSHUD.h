// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSMovementComponent.h"
#include "RTSHUD.generated.h"

/**
 * 
 */

UCLASS()
class COLONYBUILDER_API ARTSHUD : public AHUD
{
	GENERATED_BODY()

	void DrawHUD() override;

public:	
	void SetEdgeBands(FEdgeBands InBands) { EdgeBanding = InBands; }
	void SetStoredMousePos(FVector2D InMousePos, bool InDrawDebugPos) { StoredMousePos = InMousePos; DrawStoredMousePos = InDrawDebugPos; }

private:
	FEdgeBands EdgeBanding;

	FVector2D StoredMousePos;
	bool DrawStoredMousePos;
};
