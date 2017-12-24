// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuildComponent();

	//Getters//
	bool GetEnabled() { return IsEnabled; }
	//Setters//
	void SetEnabled(bool NewEnabled) { IsEnabled = NewEnabled; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	
	bool IsEnabled;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
