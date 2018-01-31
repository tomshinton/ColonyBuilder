// ColonyBuilder Project, personal project by Tom Shinton

#include "BuildComponent.h"


// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBuildComponent::StartBuildingFromClass(TSubclassOf<ABuildableBase> InClass)
{
	SetEnabled(true);


}


