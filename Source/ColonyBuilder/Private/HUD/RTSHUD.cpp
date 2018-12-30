// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSHUD.h"
#include "Kismet/GameplayStatics.h"

void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	if (DrawStoredMousePos)
	{
		float MouseX;
		float MouseY;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MouseX, MouseY);

		DrawRect(FColor::Green, StoredMousePos.X - 2.5, StoredMousePos.Y - 2.5, 5, 5);
		DrawLine(StoredMousePos.X, StoredMousePos.Y, MouseX, MouseY, FColor(0,0,255, 100), 2);
	}
}

void ARTSHUD::BeginPlay()
{
	if (HudClass)
	{
		if (UWorld* World = GetWorld())
		{
			SpawnedHUD = CreateWidget<UHudBase>(World, HudClass);
			
			if(SpawnedHUD.IsValid())
			{
				SpawnedHUD->AddToViewport(1);
			}
		}
	}
}

