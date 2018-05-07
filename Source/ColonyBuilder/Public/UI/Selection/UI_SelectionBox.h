// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SelectionBox.generated.h"

UCLASS()
class COLONYBUILDER_API UUI_SelectionBox : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSelectedActor(AActor* InSelectedActor) { SelectedActor = InSelectedActor; }
	
	UFUNCTION(BlueprintPure, Category = "Actor")
	AActor* GetSelectedActor() { return SelectedActor; }

	UFUNCTION(BlueprintNativeEvent, Category = "Update")
	void OnConstructionUpdated(const float& ConstructionPercentage);

	UFUNCTION(BlueprintNativeEvent, Category = "Update")
	void OnConstructionFinished();

private:

	AActor* SelectedActor;
};
