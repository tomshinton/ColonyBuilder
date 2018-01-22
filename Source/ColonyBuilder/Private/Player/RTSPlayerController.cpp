// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSPlayerController.h"
#include "RTSMovementComponent.h"
#include "ColonyBuilderGameModeBase.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"


ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
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
}

void ARTSPlayerController::UpdateMousePositions()
{
	GEngine->ClearOnScreenDebugMessages();
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Updating mouse world projection");
	FVector PosUnderMouseRaw;
	DeprojectMousePositionToWorld(PosUnderMouseRaw, DirUnderMouse);

	FHitResult HitRes;

	FVector TraceStart = PosUnderMouseRaw;
	FCollisionQueryParams TraceParams;
	FVector TraceEnd = PosUnderMouseRaw + (DirUnderMouse * 10000);

	ReferenceActor->ActorLineTraceSingle(HitRes, TraceStart, TraceEnd, ECC_Camera, TraceParams);

	if(HitRes.Actor.Get())
	{
		PosUnderMouse = HitRes.Location;
		//Debug
		DrawDebugSphere(GetWorld(), PosUnderMouse, 20, 6, FColor::Emerald);
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

	FVector PosUnderMouseRounded = FVector(XOut, YOut, ZOut);
	FString RoundedCallbackX = "Raw X : " + FString::SanitizeFloat(PosUnderMouse.X) + " Rounded X: " + FString::SanitizeFloat(PosUnderMouseRounded.X);
	FString RoundedCallbackY = "Raw Y : " + FString::SanitizeFloat(PosUnderMouse.Y) + " Rounded X: " + FString::SanitizeFloat(PosUnderMouseRounded.Y);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, RoundedCallbackX);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, RoundedCallbackY);
	DrawDebugSphere(GetWorld(), PosUnderMouseRounded, 20, 6, FColor::Blue);

}
