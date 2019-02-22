// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSMovementComponent.h"

#include "HUD/RTSHUD.h"

#include "PlayerPawn.h"
#include "RTSPlayerController.h"

DEFINE_LOG_CATEGORY(MovementLog);

const FName URTSMovementComponent::FloorTag("Floor");
const float URTSMovementComponent::CamBlendFrequency(0.02);

URTSMovementComponent::URTSMovementComponent()
	: Super()
	, IsUsingStaticZ(true)
	, StaticZHeight(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URTSMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(BlendCameraZoomTimer, this, &URTSMovementComponent::BlendCameraZoom, CamBlendFrequency, true);
	}

	if (OwningPawn)
	{
		Bind();
	}
}

void URTSMovementComponent::Bind()
{
	//OwningPawn->OnMoveForward.BindUObject(this, &URTSMovementComponent::MoveForwards);

	OwningPawn->OnMoveForward.CreateLambda([this](const float InStrength)
	{
		MoveForwards(InStrength);
	});

	OwningPawn->OnMoveRight.BindUObject(this, &URTSMovementComponent::MoveRight);
	OwningPawn->OnTurn.BindUObject(this, &URTSMovementComponent::Turn);
	OwningPawn->OnMouseLocationStored.BindUObject(this, &URTSMovementComponent::StoreMouseCoords);
	OwningPawn->OnMouseLocationCleared.BindUObject(this, &URTSMovementComponent::ClearMouseCoords);
	OwningPawn->OnScrollDown.BindUObject(this, &URTSMovementComponent::ZoomIn);
	OwningPawn->OnScrollUp.BindUObject(this, &URTSMovementComponent::ZoomOut);

	OwningPawn->OnMouseMoved.RemoveDynamic(this, &URTSMovementComponent::MouseMoved);
	OwningPawn->OnMouseMoved.AddDynamic(this, &URTSMovementComponent::MouseMoved);
}

void URTSMovementComponent::BuildEdgeBands()
{
	FVector2D VPSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	EdgeBands.TopBand.Height = EdgePadding_Major;
	EdgeBands.TopBand.Width = VPSize.X;
	EdgeBands.TopBand.XLoc = 0;
	EdgeBands.TopBand.YLoc = 0;
	
	EdgeBands.BottomBand.Height = EdgePadding_Bottom;
	EdgeBands.BottomBand.Width = VPSize.X;
	EdgeBands.BottomBand.XLoc = 0;
	EdgeBands.BottomBand.YLoc = VPSize.Y - EdgePadding_Bottom;

	EdgeBands.RightBand.Height = VPSize.Y;
	EdgeBands.RightBand.Width = EdgePadding_Major;
	EdgeBands.RightBand.XLoc = VPSize.X - EdgePadding_Major;
	EdgeBands.RightBand.YLoc = 0;

	EdgeBands.LeftBand.Height = VPSize.Y;
	EdgeBands.LeftBand.Width = EdgePadding_Major;
	EdgeBands.LeftBand.XLoc = 0;
	EdgeBands.LeftBand.YLoc = 0;

	EdgeBands.TopBand.Strength = MaxEdgeMoveStrength * FMath::Clamp((1 - (CurrMousePos.Y / EdgePadding_Major)), 0.f, 1.f);
	EdgeBands.BottomBand.Strength = MaxEdgeMoveStrength * FMath::Clamp(((CurrMousePos.Y - (VPSize.Y - EdgePadding_Bottom)) / EdgePadding_Bottom), 0.f, 1.f);
	EdgeBands.RightBand.Strength = MaxEdgeMoveStrength * FMath::Clamp(((CurrMousePos.X - (VPSize.X - EdgePadding_Major)) / EdgePadding_Major), 0.f, 1.f);;
	EdgeBands.LeftBand.Strength = MaxEdgeMoveStrength * FMath::Clamp(1 - (CurrMousePos.X / EdgePadding_Major), 0.f, 1.f);

	if (!UsingMiddleMouseMovement)
	{
		MoveForwards(EdgeBands.TopBand.Strength);
		MoveForwards(-EdgeBands.BottomBand.Strength);
		MoveRight(EdgeBands.RightBand.Strength);
		MoveRight(-EdgeBands.LeftBand.Strength);
	}
	
}

void URTSMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CDT = DeltaTime;

	BuildEdgeBands();
	RotateCamera();

	/*DEBUG*/

	ARTSHUD* DebugHud = Cast<ARTSHUD>((GetWorld())->GetFirstPlayerController()->GetHUD());;
	DebugHud->SetEdgeBands(EdgeBands);
}

void URTSMovementComponent::RotateCamera()
{
	float CurrYaw = GetOwner()->GetActorRotation().Yaw;
	float NewYaw = UKismetMathLibrary::FInterpTo(CurrYaw, TargetYaw, CDT, 5);

	FRotator NewRot = GetOwner()->GetActorRotation();
	NewRot.Yaw = NewYaw;

	GetOwner()->SetActorRotation(NewRot);
}

void URTSMovementComponent::StoreMouseCoords()
{
	StoredMousePos = CurrMousePos;
	UsingMiddleMouseMovement = true;
	GetWorld()->GetTimerManager().SetTimer(MiddleMouseMoveTimer, this, &URTSMovementComponent::MiddleMouseButtonMove, CDT, true);
}

void URTSMovementComponent::ClearMouseCoords()
{
	UsingMiddleMouseMovement = false;
	GetWorld()->GetTimerManager().ClearTimer(MiddleMouseMoveTimer);

	//DEBUG
	ARTSHUD* DebugHud = Cast<ARTSHUD>((GetWorld())->GetFirstPlayerController()->GetHUD());;
	DebugHud->SetStoredMousePos(StoredMousePos, false);
}

void URTSMovementComponent::MiddleMouseButtonMove()
{
	FVector2D VPSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	float XDelta;
	float YDelta;

	XDelta = ((StoredMousePos.X - CurrMousePos.X) / VPSize.X) * MiddleMouseButtonMoveStrength;
	YDelta = ((StoredMousePos.Y - CurrMousePos.Y) / VPSize.Y) * MiddleMouseButtonMoveStrength;

	MoveForwards(YDelta);
	MoveRight(-XDelta);

	ARTSHUD* DebugHud = Cast<ARTSHUD>((GetWorld())->GetFirstPlayerController()->GetHUD());;
	DebugHud->SetStoredMousePos(StoredMousePos, true);
}

void URTSMovementComponent::MoveForwards(float InAxis)
{
	FVector NewXY = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * (MoveSpeed * InAxis));
	NewXY.Z = GetAppropriateZ(NewXY);

	GetOwner()->SetActorLocation(NewXY);
}

void URTSMovementComponent::MoveRight(float InAxis)
{
	FVector NewXY = GetOwner()->GetActorLocation() + (GetOwner()->GetActorRightVector() * (MoveSpeed * InAxis));
	NewXY.Z = GetAppropriateZ(NewXY);

	GetOwner()->SetActorLocation(NewXY);
}

void URTSMovementComponent::Turn(float InAxis)
{
	TargetYaw = GetOwner()->GetActorRotation().Yaw + RotateSpeed*InAxis;
}

void URTSMovementComponent::MouseMoved(float InAxis)
{
	float MouseX;
	float MouseY;
	OwningController->GetMousePosition(MouseX, MouseY);

	CurrMousePos.X = MouseX;
	CurrMousePos.Y = MouseY;
}

float URTSMovementComponent::GetAppropriateZ(FVector InLocation)
{
	AActor* ReferenceActor = nullptr;
	float CurrZ = GetOwner()->GetActorLocation().Z;
	float OutZ = 0;
	TArray<FHitResult> HitRes;
	FCollisionShape SphereSweep = FCollisionShape::MakeSphere(50);

	FCollisionQueryParams TraceParams;
	TraceParams.TraceTag = "GetZ";
	TraceParams.bTraceComplex = true;

	TraceParams.AddIgnoredActor(GetOwner());
	FVector TraceStart = InLocation + FVector(0, 0, 200);
	FVector TraceEnd = InLocation - FVector(0, 0, 100000);

	GetWorld()->SweepMultiByChannel(HitRes, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, SphereSweep);

#if WITH_EDITOR

	GetWorld()->DebugDrawTraceTag = "GetZ";

#endif //WITH_EDITOR

	for (FHitResult result : HitRes)
	{
		ReferenceActor = result.Actor.Get();
		if (result.Actor->ActorHasTag(URTSMovementComponent::FloorTag))
		{
			if (result.ImpactPoint.Z + HeightOffset > 0)
			{
				OutZ = result.ImpactPoint.Z + HeightOffset;
				break;
			}
			else
			{
				OutZ = 0;
			}
		}
	}

	if (ReferenceActor && !IsUsingStaticZ)
	{
		return OutZ;
	}
	else
	{
		return StaticZHeight;
	}

}

void URTSMovementComponent::ZoomIn()
{
	float CurrSpringArmLength = TargetArmLength;

	if ((CurrSpringArmLength - ArmZoomRate) >= MinArmLength)
	{
		TargetArmLength = CurrSpringArmLength - ArmZoomRate;
	}
}

void URTSMovementComponent::ZoomOut()
{
	float CurrSpringArmLength = TargetArmLength;

	if ((CurrSpringArmLength + ArmZoomRate) <= MaxArmLength)
	{
		TargetArmLength = CurrSpringArmLength + ArmZoomRate;
	}
}

void URTSMovementComponent::BlendCameraZoom()
{
	float NewArmLength;
	float CurrentLength = CameraArm->TargetArmLength;

	NewArmLength = FMath::FInterpTo(CurrentLength, TargetArmLength, CDT, CameraZoomSpeed);
	CameraArm->TargetArmLength = NewArmLength;
}

