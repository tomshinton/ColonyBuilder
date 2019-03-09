// ColonyBuilder Project, personal project by Tom Shinton
#pragma once

#include "Utils/DataTypes/SaveDataTypes.h"

#include "PlayerPawn.generated.h"

class ARTSPlayerController;
class USelectionComponent;
class UBuildComponent;
class UMoveComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class APawn;

UCLASS()
class COLONYCORE_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* PawnRoot;

	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditDefaultsOnly)
	UMoveComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly)
	UBuildComponent* BuildComponent;

	UPROPERTY(EditDefaultsOnly)
	USelectionComponent* SelectionComponent;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	ARTSPlayerController* RTSController;

	//Getters//
	UFUNCTION(BlueprintPure, Category = "Player | Modes")	
	UMoveComponent* GetRTSMovementComponent() { return MovementComp; }
	UBuildComponent* GetBuildComponent() { return BuildComponent; }

	FPlayerSaveData GetSaveData();
	void LoadSaveData(const FPlayerSaveData& LoadedData);

	//Delegates
	DECLARE_DELEGATE_OneParam(FOnMoveFoward, float);
	FOnMoveFoward OnMoveForward;

	DECLARE_DELEGATE_OneParam(FOnMoveRight, float);
	FOnMoveRight OnMoveRight;

	DECLARE_DELEGATE_OneParam(FOnTurn, float);
	FOnTurn OnTurn;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseMoved, float, InAxis);
	FOnMouseMoved OnMouseMoved;

	DECLARE_DELEGATE(FOnStoreMousePosition);
	FOnStoreMousePosition OnMouseLocationStored;

	DECLARE_DELEGATE(FOnClearMousePosition);
	FOnClearMousePosition OnMouseLocationCleared;

	DECLARE_DELEGATE(FOnScrollUp);
	FOnScrollUp OnScrollUp;

	DECLARE_DELEGATE(FOnScrollDown);
	FOnScrollDown OnScrollDown;

private:
	void MoveForward(float InAxis);
	void MoveRight(float InAxis);
	void Turn(float InAxis);

	void MouseMoved(float InAxis);

	void StoreMouseCoords();
	void ClearMouseCoords();

	void ScrollUp();
	void ScrollDown();
};
