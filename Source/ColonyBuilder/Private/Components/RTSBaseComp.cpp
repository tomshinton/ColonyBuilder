// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSBaseComp.h"
#include "PlayerPawn.h"

#include "Utils/Libraries/DebugUtils.h"

// Sets default values for this component's properties
URTSBaseComp::URTSBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URTSBaseComp::BeginPlay()
{
	Super::BeginPlay();
	
	FString ComponentCreationCallback = "Creating " + this->GetName() + " for " + OwningPawn->GetName();
	print(ComponentCreationCallback);
}
// Called every frame
void URTSBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URTSBaseComp::SetEnabled(bool InEnabled)
{
	IsEnabled = InEnabled;

	FString EnableCallback = "Setting IsEnabled in " + this->GetName() + " to " + (IsEnabled ? TEXT("True") : TEXT("False"));
	print(EnableCallback);
}
