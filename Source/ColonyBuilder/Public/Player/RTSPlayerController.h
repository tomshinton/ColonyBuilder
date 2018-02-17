// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

/**
 * 
 */

class AColonyBuilderGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMouseCoordsChanged, FVector, MouseCoordsRaw, FVector, MouseCoordsRounded);

UCLASS()
class COLONYBUILDER_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

	ARTSPlayerController();

	void BeginPlay();

public:
	UFUNCTION()
	void UpdateMousePositions(float InAxis);
	FOnMouseCoordsChanged OnMouseMoved;

#pragma region Utils
	//Getters
	FVector GetPositionUnderMouse() { return PosUnderMouse; }
	FVector GetPositionUnderMouseRounded() { return PosUnderMouseRounded; }

	//Setters

#pragma endregion Utils

private:

	FVector PosUnderMouse;
	FVector DirUnderMouse;
	FVector PosUnderMouseRounded;

	AActor* ReferenceActor;
	AColonyBuilderGameModeBase* GM;

};
