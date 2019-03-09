// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include <Button.h>
#include "UI_BuildButton.generated.h"

class UBuildComponent;
class UBuildingData;
class ARTSPlayerController;

UCLASS(DisplayName = "Build Button")
class  UUI_BuildButton : public UButton
{
	GENERATED_BODY()

	UUI_BuildButton();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	UBuildingData* BuildingData;

	UBuildComponent* BuildComponentRef;
	ARTSPlayerController* ControllerRef;

	UFUNCTION()
	void BeginBuild();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif //WITH_EDITOR

};
