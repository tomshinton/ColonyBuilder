// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class UMoveComponent;
class UHudBase;

#include <GameFramework/HUD.h>

#include "RTSHUD.generated.h"

UCLASS()
class COLONYCORE_API ARTSHUD : public AHUD
{
	GENERATED_BODY()

	void DrawHUD() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UHudBase> HudClass;

public:	
	void SetEdgeBands(FEdgeBands InBands) { EdgeBanding = InBands; }
	void SetStoredMousePos(FVector2D InMousePos, bool InDrawDebugPos) { StoredMousePos = InMousePos; DrawStoredMousePos = InDrawDebugPos; }

private:
	FEdgeBands EdgeBanding;

	FVector2D StoredMousePos;
	bool DrawStoredMousePos;

	TWeakObjectPtr<UHudBase> SpawnedHUD;
protected:
	virtual void BeginPlay() override;

};
