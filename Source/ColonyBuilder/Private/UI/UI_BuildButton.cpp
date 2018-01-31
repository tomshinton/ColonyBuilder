// ColonyBuilder Project, personal project by Tom Shinton

#include "UI_BuildButton.h"
#include "BuildComponent.h"
#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "Button"

UUI_BuildButton::UUI_BuildButton()
{
	OnClicked.AddDynamic(this, &UUI_BuildButton::BeginBuild);
}

#if WITH_EDITOR

void UUI_BuildButton::BeginBuild()
{
	if (!BuildComponentRef)
	{
		BuildComponentRef = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->GetBuildComponent();
	}

	if (BuildComponentRef)
	{
		if (ClassToBuild)
		{
			FString StartingBuildmodeCallback = "Beginning building of " + ClassToBuild->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, StartingBuildmodeCallback);

			BuildComponentRef->StartBuildingFromClass(ClassToBuild);
		}
	}
}

const FText UUI_BuildButton::GetPaletteCategory()
{
	return LOCTEXT("", "Colony Elements | Buttons");
}

#endif

#undef LOCTEXT_NAMESPACE