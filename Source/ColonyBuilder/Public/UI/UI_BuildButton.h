// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "BuildComponent.h"

#include "UI_BuildButton.generated.h"

class UBuildComponent;
class UBuildingData;

/**
 * 
 */
UCLASS(DisplayName = "Build Button")
class COLONYBUILDER_API UUI_BuildButton : public UButton
{
	GENERATED_BODY()

	UUI_BuildButton();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building")
	UBuildingData* BuildingData;

	UBuildComponent* BuildComponentRef;
	
	UFUNCTION()
	void BeginBuild();

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif //WITH_EDITOR

};
