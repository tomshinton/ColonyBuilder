// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSHUD.generated.h"

/**
 * 
 */

UCLASS()
class COLONYBUILDER_API ARTSHUD : public AHUD
{
	GENERATED_BODY()

	void DrawHUD();
public:

	void DrawDebugHUD();

	//Getters//
	bool GetDebugHUD() { return DebugHUD;  }
	//Setters//
	void SetDebugHUD(bool newDebugHUD) { DebugHUD = newDebugHUD; }

private:
	bool DebugHUD = true;
	
	
};
