// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSPlayerController.h"
#include "RTSMovementComponent.h"
#include "ColonyBuilderGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "Utils/Cheats/ColonyCheatManager.h"


ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	CheatClass = UColonyCheatManager::StaticClass();
}

void ARTSPlayerController::BeginPlay()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	for (TActorIterator<AActor> Itr(GetWorld()); Itr; ++Itr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AActor *Ref = *Itr;
		if (Ref->ActorHasTag(URTSMovementComponent::FloorTag))
		{
			ReferenceActor = Ref;
			break;
		}
	}

	GM = Cast<AColonyBuilderGameModeBase>(GetWorld()->GetAuthGameMode());

	if (APlayerPawn* OwningPawn = Cast<APlayerPawn>(GetPawn()))
	{
		OwningPawn->OnMouseMoved.AddDynamic(this, &ARTSPlayerController::UpdateMousePositions);
	}
}

void ARTSPlayerController::UpdateMousePositions(float InAxis)
{
	FVector PosUnderMouseRaw;
	DeprojectMousePositionToWorld(PosUnderMouseRaw, DirUnderMouse);

	FHitResult HitRes;

	FVector TraceStart = PosUnderMouseRaw;
	FCollisionQueryParams TraceParams;
	FVector TraceEnd = PosUnderMouseRaw + (DirUnderMouse * 50000);

	ReferenceActor->ActorLineTraceSingle(HitRes, TraceStart, TraceEnd, ECC_Camera, TraceParams);

	if(HitRes.Actor.Get())
	{
		PosUnderMouse = HitRes.Location;
	}

	//Rounding
	//X
	int32 XIn = FMath::Abs(FMath::RoundToInt(PosUnderMouse.X));
	int32 XDif = FMath::Abs(XIn%AColonyBuilderGameModeBase::GridSize);
	float XOut;
	if (XDif < AColonyBuilderGameModeBase::GridSize / 2)
	{
		XOut = XIn - XDif;
	}
	else
	{
		XOut = XIn + (AColonyBuilderGameModeBase::GridSize - XDif);
	}

	int32 YIn = FMath::Abs(FMath::RoundToInt(PosUnderMouse.Y));
	int32 YDif = FMath::Abs(YIn%AColonyBuilderGameModeBase::GridSize);
	float YOut;
	if (YDif < AColonyBuilderGameModeBase::GridSize / 2)
	{
		YOut = YIn - YDif;
	}
	else
	{
		YOut = YIn + (AColonyBuilderGameModeBase::GridSize - YDif);
	}

	if (PosUnderMouse.Y <= 0)
	{
		YOut *= -1;
	}
	if (PosUnderMouse.X <= 0)
	{
		XOut *= -1;
	}

	float ZOut = PosUnderMouse.Z;
	ReferenceActor->ActorLineTraceSingle(HitRes, FVector(XOut, YOut, 10000), FVector(XOut, YOut, -1000), ECC_Camera, TraceParams);

	if (HitRes.Actor.Get())
	{
		ZOut = HitRes.Location.Z;
	}

	PosUnderMouseRounded = FVector(XOut, YOut, ZOut);
	OnMouseMoved.Broadcast(PosUnderMouse, PosUnderMouseRounded);
}
