// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/HUD.h"

#include "RTSMovementComponent.generated.h"


USTRUCT(BlueprintType)
struct FScreenEdge
{
	GENERATED_USTRUCT_BODY()

	float XLoc;
	float YLoc;

	float Width;
	float Height;

	float Strength;

	void DrawBand(UWorld* WorldContext)
	{
		FLinearColor BandColour = FLinearColor(1, 0, 0, Strength);
		AHUD* DebugHud = Cast<AHUD>((WorldContext)->GetFirstPlayerController()->GetHUD());;
		DebugHud->DrawRect(BandColour, XLoc, YLoc, Width, Height);
	}
};

USTRUCT(BlueprintType)
struct FEdgeBands
{
	GENERATED_BODY()

	FScreenEdge TopBand;
	FScreenEdge BottomBand;
	FScreenEdge RightBand;
	FScreenEdge LeftBand;

	void DrawBands(UWorld* WorldContext)
	{
		TopBand.DrawBand(WorldContext);
		BottomBand.DrawBand(WorldContext);
		RightBand.DrawBand(WorldContext);
		LeftBand.DrawBand(WorldContext);
	}
};

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
	void BuildEdgeBands();
	float GetAppropriateZ(FVector InLocation);

	//Delta time, useful for printing
	float CDT;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Getters//
	FEdgeBands GetEdgeBands() {	return EdgeBands; }

	void MoveForwards(float InAxis);
	void MoveRight(float InAxis);
	void Turn(float InAxis);
	void MouseMoved(float InAxis);

	float RotateSpeed;
	float MoveSpeed;
	float HeightOffset;
	
	float EdgePadding_Major = 100;
	float EdgePadding_Bottom = 100;

	float MaxEdgeMoveStrength = 1.f;

private:

	void RotateCamera();

	float TargetYaw;

	APawn* OwningPawn;
	APlayerController* OwningController;

	FVector2D CurrMousePos;
	FEdgeBands EdgeBands;

};
