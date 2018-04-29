// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTSBaseComp.generated.h"

class APlayerPawn;
class ARTSPlayerController;

UENUM(BlueprintType)
enum class EComponentFunctionType : uint8
{
	Function	UMETA(DisplayName = "Functional Component"),
	Navigation	UMETA(DisplayName = "Navigational Component")
};

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
	ARTSPlayerController* OwningController;

	bool IsEnabled;
	EComponentFunctionType ComponentType;
};
