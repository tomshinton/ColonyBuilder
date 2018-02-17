// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utils.h"
#include "RTSBaseComp.generated.h"

class APlayerPawn;

DECLARE_LOG_CATEGORY_EXTERN(BaseComponentLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API URTSBaseComp : public UActorComponent
{
	GENERATED_BODY()

public:	

	URTSBaseComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetOwningPlayer(APlayerPawn* InPlayer) { OwningPawn = InPlayer; }
	
	virtual void SetEnabled(bool InEnabled);
	bool GetEnabled() { return IsEnabled; }
	
protected:
	virtual void BeginPlay() override;

	APlayerPawn* OwningPawn;
	bool IsEnabled;
};
