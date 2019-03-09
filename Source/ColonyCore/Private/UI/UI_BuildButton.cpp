// ColonyBuilder Project, personal project by Tom Shinton

#include "UI_BuildButton.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/Libraries/DebugUtils.h"

#include "Image.h"

#define LOCTEXT_NAMESPACE "Button"

UUI_BuildButton::UUI_BuildButton()
{
	OnClicked.AddDynamic(this, &UUI_BuildButton::BeginBuild);
}


void UUI_BuildButton::BeginBuild()
{
	if (!BuildComponentRef || !ControllerRef)
	{
		ControllerRef = Cast<ARTSPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (ControllerRef)
		{
			BuildComponentRef = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->GetBuildComponent();
		}
	}

	if (BuildComponentRef && ControllerRef)
	{
		if (BuildingData)
		{
			FString StartingBuildmodeCallback = "Beginning building of " + BuildingData->GetName();
			print(StartingBuildmodeCallback);

			BuildComponentRef->StartBuildingFromClass(BuildingData);
		}
	}
}

#if WITH_EDITOR

const FText UUI_BuildButton::GetPaletteCategory()
{
	return LOCTEXT("", "Colony Elements | Buttons");
}

#endif

#undef LOCTEXT_NAMESPACE