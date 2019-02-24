// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSMovementComponent.h"

#include "PlayerPawn.h"
#include "RTSPlayerController.h"
#include <DrawDebugHelpers.h>

DEFINE_LOG_CATEGORY(MovementLog);

const float URTSMovementComponent::MoveFrequency(0.01);
const float URTSMovementComponent::StaticZHeight(0.f);

URTSMovementComponent::URTSMovementComponent()
	: Super()
	, CameraArm(nullptr)
	, EdgePadding_Major(20.f) // Edge padding pixels on right, left and top
	, EdgePadding_Bottom(10.f) //Edge padding pixels on the bottom
	, MaxEdgeMoveStrength(1.f)
	, MiddleMouseButtonMoveStrength(1.f)
	, MaxArmLength(10000.f)
	, MinArmLength(1000.f)
	, ArmZoomRate(1000.f)
	, CameraZoomSpeed(2.f)
	, TargetArmLength(MaxArmLength)
	, MoveSpeed(1.f)
	, RotateSpeed(1.f)
	, HeightOffset(0.f)
	, XLimit()
	, YLimit()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URTSMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		FTimerDelegate RotateDelegate;
		RotateDelegate.BindUFunction(this, FName("RotateCamera"), MoveFrequency);

		World->GetTimerManager().SetTimer(EdgeMoveHandle, this, &URTSMovementComponent::EdgeMove, MoveFrequency, true);
		World->GetTimerManager().SetTimer(ZoomCameraHandle, this, &URTSMovementComponent::BlendCameraZoom, MoveFrequency, true);
		World->GetTimerManager().SetTimer(RotateCameraHandle, RotateDelegate, MoveFrequency, true);
	}

	if (OwningPawn)
	{
		Bind();

		if (OwningController)
		{
			SetMovementLimits(*OwningController->GetReferenceActor());
		}
	}

	if (FViewport* Viewport = GEngine->GameViewport->Viewport)
	{
		SetViewportSize(Viewport->GetSizeXY());

		Viewport->ViewportResizedEvent.AddLambda([this](FViewport* ResizedViewport, uint32)
		{
			SetViewportSize(ResizedViewport->GetSizeXY());
		});
	}

	EdgeBands.TopBand = FScreenEdge(0.f, 0.f, ViewportSize.X, ViewportSize.Y);
	EdgeBands.BottomBand = FScreenEdge(0.f, ViewportSize.Y - EdgePadding_Bottom, ViewportSize.X, EdgePadding_Bottom);
	EdgeBands.RightBand = FScreenEdge(ViewportSize.X - EdgePadding_Major, 0, EdgePadding_Major, ViewportSize.Y);
	EdgeBands.LeftBand = FScreenEdge(0.f, 0.f, EdgePadding_Major, ViewportSize.Y);
}

void URTSMovementComponent::SetViewportSize(const FVector2D InNewViewportSize)
{
	ViewportSize = InNewViewportSize;
}

void URTSMovementComponent::Bind()
{
	UWorld* World = GetWorld();

	if(!World)
	{
		return;
	}

	OwningPawn->OnMoveForward.BindUObject(this, &URTSMovementComponent::MoveForwards);
	OwningPawn->OnMoveRight.BindUObject(this, &URTSMovementComponent::MoveRight);
	OwningPawn->OnTurn.BindUObject(this, &URTSMovementComponent::Turn);

	OwningPawn->OnMouseLocationStored.BindLambda([this, World = World]() 
	{
		StoredMousePos = CurrMousePos;
		UsingMiddleMouseMovement = true;
		World->GetTimerManager().SetTimer(MiddleMouseMoveTimer, this, &URTSMovementComponent::MiddleMouseButtonMove, MoveFrequency, true);
	});

	OwningPawn->OnMouseLocationCleared.BindLambda([this, World = World]()
	{
		UsingMiddleMouseMovement = false;
		GetWorld()->GetTimerManager().ClearTimer(MiddleMouseMoveTimer);
	});

	OwningPawn->OnScrollDown.BindUObject(this, &URTSMovementComponent::ZoomIn);
	OwningPawn->OnScrollUp.BindUObject(this, &URTSMovementComponent::ZoomOut);

	OwningPawn->OnMouseMoved.AddDynamic(this, &URTSMovementComponent::MouseMoved);
}

void URTSMovementComponent::EdgeMove()
{
	EdgeBands.TopBand.SetStrength(MaxEdgeMoveStrength * FMath::Clamp((1 - (CurrMousePos.Y / EdgePadding_Major)), 0.f, 1.f));
	EdgeBands.BottomBand.SetStrength(MaxEdgeMoveStrength * FMath::Clamp(((CurrMousePos.Y - (ViewportSize.Y - EdgePadding_Bottom)) / EdgePadding_Bottom), 0.f, 1.f));
	EdgeBands.RightBand.SetStrength(MaxEdgeMoveStrength * FMath::Clamp(((CurrMousePos.X - (ViewportSize.X - EdgePadding_Major)) / EdgePadding_Major), 0.f, 1.f));
	EdgeBands.LeftBand.SetStrength(MaxEdgeMoveStrength * FMath::Clamp(1 - (CurrMousePos.X / EdgePadding_Major), 0.f, 1.f));

	if (!UsingMiddleMouseMovement)
	{
		MoveForwards(EdgeBands.TopBand.GetStrength());
		MoveForwards(-EdgeBands.BottomBand.GetStrength());
		MoveRight(EdgeBands.RightBand.GetStrength());
		MoveRight(-EdgeBands.LeftBand.GetStrength());
	}
}

void URTSMovementComponent::SetMovementLimits(const AActor& ReferenceActor)
{
	FVector Origin;
	FVector Bounds;

	ReferenceActor.GetActorBounds(true, Origin, Bounds);

	XLimit = FVector2D(Origin.X - Bounds.X, Origin.X + Bounds.X);
	YLimit = FVector2D(Origin.Y - Bounds.Y, Origin.Y + Bounds.Y);

	DrawDebugBox(GetWorld(), Origin, Bounds, FQuat(ReferenceActor.GetActorRotation()), FColorList::HunterGreen, true);
}

void URTSMovementComponent::RotateCamera(const float InRotateDelta)
{
	if (OwningPawn)
	{
		const float CurrYaw = GetOwner()->GetActorRotation().Yaw;
		const float NewYaw = UKismetMathLibrary::FInterpTo(CurrYaw, TargetYaw, InRotateDelta, 5);

		const FRotator CurrRotation = OwningPawn->GetActorRotation();
		const FRotator NewRotation = FRotator(CurrRotation.Pitch, NewYaw, CurrRotation.Roll);
		OwningPawn->SetActorRotation(NewRotation);
	}
}

void URTSMovementComponent::MiddleMouseButtonMove()
{
	MoveRight(-(((StoredMousePos.X - CurrMousePos.X) / ViewportSize.X) * MiddleMouseButtonMoveStrength));
	MoveForwards(((StoredMousePos.Y - CurrMousePos.Y) / ViewportSize.Y) * MiddleMouseButtonMoveStrength);
}

void URTSMovementComponent::MoveForwards(float InAxis)
{
	if (OwningPawn)
	{
		const FVector2D NewXY(OwningPawn->GetActorLocation() + (OwningPawn->GetActorForwardVector() * (MoveSpeed * InAxis)));
		SetLimitedActorLocation(FVector(NewXY, StaticZHeight));
	}
}

void URTSMovementComponent::MoveRight(float InAxis)
{
	if (OwningPawn)
	{
		const FVector2D NewXY(OwningPawn->GetActorLocation() + (OwningPawn->GetActorRightVector() * (MoveSpeed * InAxis)));
		SetLimitedActorLocation(FVector(NewXY, StaticZHeight));
	}
}

void URTSMovementComponent::Turn(float InAxis)
{
	if (OwningPawn)
	{
		TargetYaw = OwningPawn->GetActorRotation().Yaw + RotateSpeed*InAxis;
	}
}

void URTSMovementComponent::MouseMoved(float InAxis)
{
	float MouseX;
	float MouseY;
	OwningController->GetMousePosition(MouseX, MouseY);

	CurrMousePos.X = MouseX;
	CurrMousePos.Y = MouseY;
}

void URTSMovementComponent::ZoomIn()
{
	TargetArmLength = FMath::Clamp<float>(TargetArmLength - ArmZoomRate, MinArmLength, MaxArmLength);
}

void URTSMovementComponent::ZoomOut()
{
	TargetArmLength = FMath::Clamp<float>(TargetArmLength + ArmZoomRate, MinArmLength, MaxArmLength);
}

void URTSMovementComponent::BlendCameraZoom()
{
	const float CurrentLength = CameraArm->TargetArmLength;

	const float NewArmLength = FMath::FInterpTo(CurrentLength, TargetArmLength, MoveFrequency, CameraZoomSpeed);
	CameraArm->TargetArmLength = NewArmLength;
}

void URTSMovementComponent::SetLimitedActorLocation(const FVector& NewLocation)
{
	if (OwningPawn)
	{
		const FVector ClampedLocation(FMath::Clamp(NewLocation.X, XLimit.X, XLimit.Y), FMath::Clamp(NewLocation.Y, YLimit.X, YLimit.Y), NewLocation.Z);
		OwningPawn->SetActorLocation(ClampedLocation);
	}
}

