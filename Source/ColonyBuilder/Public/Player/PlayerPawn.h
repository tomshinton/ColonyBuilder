// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "RTSMovementComponent.h"
#include "BuildComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class ARTSPlayerController;

DECLARE_DELEGATE_OneParam(FOnMoveFoward, float);
DECLARE_DELEGATE_OneParam(FOnMoveRight, float);
DECLARE_DELEGATE_OneParam(FOnTurn, float);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseMoved, float, InAxis);

DECLARE_DELEGATE(FOnStoreMousePosition);
DECLARE_DELEGATE(FOnClearMousePosition);
DECLARE_DELEGATE(FOnScrollUp);
DECLARE_DELEGATE(FOnScrollDown);

DECLARE_DELEGATE(FOnRotatePlacement);
DECLARE_DELEGATE(FOnConfirmAction);
DECLARE_DELEGATE(FOnCancelAction);

UCLASS()
class COLONYBUILDER_API APlayerPawn : public APawn
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
	URTSMovementComponent* MovementComp;
	UPROPERTY(EditDefaultsOnly)
	UBuildComponent* BuildComponent;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ARTSPlayerController* RTSController;

	//Getters//
	UFUNCTION(BlueprintPure, Category = "Player | Modes")	URTSMovementComponent* GetRTSMovementComponent() { return MovementComp; }
	UBuildComponent* GetBuildComponent() { return BuildComponent; }

	//Delegates
	FOnMoveFoward OnMoveForward;
	FOnMoveRight OnMoveRight;
	FOnTurn OnTurn;
	FOnMouseMoved OnMouseMoved;

	FOnStoreMousePosition OnMouseLocationStored;
	FOnClearMousePosition OnMouseLocationCleared;
	FOnScrollUp OnScrollUp;
	FOnScrollDown OnScrollDown;

	FOnRotatePlacement OnRotatePlacement;
	FOnConfirmAction OnConfirmAction;
	FOnCancelAction OnCancelAction;

private:
	void MoveForward(float InAxis);
	void MoveRight(float InAxis);
	void Turn(float InAxis);

	void MouseMoved(float InAxis);

	void StoreMouseCoords();
	void ClearMouseCoords();

	void ScrollUp();
	void ScrollDown();

	void RotatePlacement();
	void Confirm();
	void Cancel();

};
