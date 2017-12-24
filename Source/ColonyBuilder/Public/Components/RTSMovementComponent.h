// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "RTSMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(MovementLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API URTSMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	float GetAppropriateZ(FVector InLocation);

	//Delta time, useful for printing
	float CDT;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveForwards(float InAxis);
	void MoveRight(float InAxis);
	void Turn(float InAxis);

	float RotateSpeed;
	float MoveSpeed;
	float HeightOffset;
	
private:

	void RotateCamera();
	float TargetYaw;

};
