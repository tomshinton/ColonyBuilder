// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSPlayerController.h"

#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

#include "Utils/Cheats/ColonyCheatManager.h"
#include "Utils/Libraries/CollisionChannels.h"

DEFINE_LOG_CATEGORY(RTSControllerLog);

const FName ARTSPlayerController::FloorTag("Floor");
const float ARTSPlayerController::ScreenTraceDepth(50000.f);

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
		if (AActor* Ref = *Itr)
		{
			if (Ref->ActorHasTag(ARTSPlayerController::FloorTag))
			{
				ReferenceActor = Ref;
				break;
			}
		}
	}

	if (!ReferenceActor)
	{
		UE_LOG(RTSControllerLog, Error, TEXT("Could not find Floor actor in BeginPlay of RTSController!"));
	}

	if (UWorld* World = GetWorld())
	{
		GM = Cast<AColonyBuilderGameModeBase>(GetWorld()->GetAuthGameMode());

		if (APlayerPawn* OwningPawn = Cast<APlayerPawn>(GetPawn()))
		{
			OwningPawn->OnMouseMoved.AddDynamic(this, &ARTSPlayerController::UpdateMousePositions);
		}
	}
}

void ARTSPlayerController::UpdateMousePositions(float InAxis)
{
	if (UWorld* World = GetWorld())
	{
		FVector PosUnderMouseRaw;
		FVector DirUnderMouse;
		DeprojectMousePositionToWorld(PosUnderMouseRaw, DirUnderMouse);

		const FVector StartPoint = PosUnderMouseRaw;
		const FVector EndPoint = PosUnderMouseRaw + (DirUnderMouse * ScreenTraceDepth);
		FHitResult HitRes;

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceAsyncScene = true;

		FCollisionResponseParams ResponseParams;
		World->AsyncLineTraceByChannel(EAsyncTraceType::Single, StartPoint, EndPoint, CC_GROUND, QueryParams, ResponseParams, &OnTraceCompleteDelegate);

		OnTraceCompleteDelegate.BindLambda([this](const FTraceHandle& InHandle, FTraceDatum& InData)
		{
			if (InData.OutHits.Num() > 0)
			{
				FHitResult& CurrHit = InData.OutHits[0];

				if (AActor* HitActor = CurrHit.Actor.Get())
				{
					if (HitActor == ReferenceActor)
					{
						RoundHitResult(CurrHit.Location);
					}
				}
			}
		});
	}
}

void ARTSPlayerController::RoundHitResult(const FVector HitLocation)
{
	const int32 XIn = FMath::Abs(FMath::RoundToInt(HitLocation.X));
	const int32 XDif = FMath::Abs(XIn%AColonyBuilderGameModeBase::GridSize);
	const float XOut = (XDif < AColonyBuilderGameModeBase::GridSize * 0.5f ? XIn - XDif : XIn + (AColonyBuilderGameModeBase::GridSize - XDif)) * FMath::Sign<float>(HitLocation.X);

	const int32 YIn = FMath::Abs(FMath::RoundToInt(HitLocation.Y));
	const int32 YDif = FMath::Abs(YIn%AColonyBuilderGameModeBase::GridSize);
	const float YOut = (YDif < AColonyBuilderGameModeBase::GridSize * 0.5f ? YIn - YDif :  YIn + (AColonyBuilderGameModeBase::GridSize - YDif)) * FMath::Sign<float>(HitLocation.Y);

	PosUnderMouseRounded = FVector(XOut, YOut, HitLocation.Z);
	OnMouseMoved.Broadcast(HitLocation, PosUnderMouseRounded);
}

