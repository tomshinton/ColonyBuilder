// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

class AColonyBuilderGameModeBase;
class APlayerController;

#include "RTSPlayerController.generated.h"

//////////////////////////////////////////////////////////////////////////
// Base controller for RTS Player Pawn, controls mouse based tracing mostly
//////////////////////////////////////////////////////////////////////////

DECLARE_LOG_CATEGORY_EXTERN(RTSControllerLog, Log, All);

UCLASS()
class COLONYCORE_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARTSPlayerController();

	void BeginPlay();

public:

	UFUNCTION()
	void UpdateMousePositions(float InAxis);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMouseCoordsChanged, const FVector&, GroundLocationUnderMouse, const FVector&, RoundedGroundLocationUnderMouse);
	FOnMouseCoordsChanged OnMouseMoved;

	FVector GetPositionUnderMouseRounded() { return PosUnderMouseRounded; }
	AActor* GetReferenceActor() { return ReferenceActor; }

private:

	void RoundHitResult(const FVector HitLocation);

	FVector PosUnderMouseRounded;

	UPROPERTY()
	AActor* ReferenceActor;

	UPROPERTY()
	AColonyBuilderGameModeBase* GM;

	static const FName FloorTag;
	static const float ScreenTraceDepth;

	FTraceDelegate OnTraceCompleteDelegate;
};
