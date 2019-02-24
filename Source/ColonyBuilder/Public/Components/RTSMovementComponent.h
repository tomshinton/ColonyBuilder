// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/HUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "RTSBaseComp.h"

#include "RTSMovementComponent.generated.h"

//////////////////////////////////////////////////////////////////////////
// RTS Movement class, controls how the pawn moves, zooms, and scrolls around the viewport
//////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FScreenEdge
{
	GENERATED_BODY()

	FScreenEdge()
		: XLoc(0.f)
		, YLoc(0.f)
		, Width(0.f)
		, Height(0.f)
		, Strength(0.f)
	{}

	FScreenEdge(const float InXLoc, const float InYLoc, const float InWidth, const float InHeight)
		: XLoc(InXLoc)
		, YLoc(InYLoc)
		, Width(InWidth)
		, Height(InHeight)
		, Strength(0.f)
	{}

public:

	void SetStrength(const float InStrength) { Strength = InStrength; };
	float GetStrength() const { return Strength; };

private:
	float XLoc;
	float YLoc;

	float Width;
	float Height;

	float Strength;
};

USTRUCT(BlueprintType)
struct FEdgeBands
{
	GENERATED_BODY()

public:
	FScreenEdge TopBand;
	FScreenEdge BottomBand;
	FScreenEdge RightBand;
	FScreenEdge LeftBand;
};

DECLARE_LOG_CATEGORY_EXTERN(MovementLog, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API URTSMovementComponent : public URTSBaseComp
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSMovementComponent();

	void BeginPlay() override;

	FEdgeBands GetEdgeBands() { return EdgeBands; }

	void MoveForwards(float InAxis);
	void MoveRight(float InAxis);
	void Turn(float InAxis);

	UFUNCTION()
	void MouseMoved(float InAxis);

	UPROPERTY()
	USpringArmComponent* CameraArm;

	float EdgePadding_Major;
	float EdgePadding_Bottom;

	float MaxEdgeMoveStrength;
	float MiddleMouseButtonMoveStrength;

	float MaxArmLength;
	float MinArmLength;
	float ArmZoomRate;
	float CameraZoomSpeed;
	float TargetArmLength;

	float MoveSpeed;
	float RotateSpeed;
	float HeightOffset;

	void ZoomIn();
	void ZoomOut();
	
private:

	void Bind();
	void EdgeMove();

	void SetPlayerLocation();

	UFUNCTION()
	void RotateCamera(const float InRotationDelta);

	void MiddleMouseButtonMove();
	void BlendCameraZoom();

	void SetViewportSize(const FVector2D InNewViewportSize);
	void SetMovementLimits(const AActor& ReferenceActor);

	void SetLimitedActorLocation(const FVector& NewLocation);

	FVector2D YLimit;
	FVector2D XLimit;

	float TargetYaw;

	FVector2D CurrMousePos;
	FVector2D StoredMousePos;
	FVector2D ViewportSize;
	bool UsingMiddleMouseMovement;

	FEdgeBands EdgeBands;
	
	FTimerHandle RotateCameraHandle;
	FTimerHandle EdgeMoveHandle;
	FTimerHandle ZoomCameraHandle;
	FTimerHandle MiddleMouseMoveTimer;

	static const float MoveFrequency;
	static const float StaticZHeight;
};
