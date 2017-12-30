// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSHUD.h"
#include "PlayerPawn.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	EdgeBanding.DrawBands(GetWorld());
}

