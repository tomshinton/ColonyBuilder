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

UENUM(BlueprintType)
enum class EMovementType : uint8 {

	RTS		UMETA(DisplayName = "RTS Movement")
};

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Getters//
	EMovementType GetCurrMovementType() { return CurrMovementType; }
	URTSMovementComponent* GetRTSMovementComponent() { return MovementComp; }

	//Setters//
	void SetCurrMovementType(EMovementType NewMovemementType) { CurrMovementType = NewMovemementType; }

private:
	void MoveForward(float InAxis);
	void MoveRight(float InAxis);
	void Turn(float InAxis);
	void MouseMoved(float InAxis);

	EMovementType CurrMovementType;

};
