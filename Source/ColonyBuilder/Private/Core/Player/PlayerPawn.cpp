// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "RTSPlayerController.h"

#include "SaveManager.h"
#include "ColonyInstance.h"
#include "ColonySave.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Root Component"));
	PawnRoot->SetCollisionProfileName("NoCollision");
	RootComponent = PawnRoot;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(PawnRoot);
	SpringArm->SetRelativeRotation(FRotator(-45, 0, 0));
	SpringArm->TargetArmLength = 5000;
	SpringArm->bDoCollisionTest = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	PlayerCamera->SetupAttachment(SpringArm);

	MovementComp = CreateDefaultSubobject<URTSMovementComponent>(TEXT("RTS Movement Component"));
	MovementComp->SetOwningPlayer(this);
	MovementComp->MoveSpeed = 35;
	MovementComp->RotateSpeed = 12;
	MovementComp->HeightOffset = PawnRoot->GetScaledSphereRadius();

	MovementComp->CameraArm = SpringArm;

	BuildComponent = CreateDefaultSubobject<UBuildComponent>(TEXT("Build Component"));
	BuildComponent->SetOwningPlayer(this);

	SelectionComponent = CreateDefaultSubobject<USelectionComponent>(TEXT("Selection Component"));
	SelectionComponent->SetOwningPlayer(this);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	RebindNavigationComponents();

	MovementComp->SetEnabled(true);
	SelectionComponent->SetEnabled(true);

	//Load any saved data, if there is any
	if (UColonyManager* Manager = Cast<UColonyInstance>(UGameplayStatics::GetGameInstance(this))->GetManagerByClass(USaveManager::StaticClass()))
	{
		if (USaveManager* SaveManager = Cast<USaveManager>(Manager))
		{
			if (SaveManager->GetCurrentSave()->PlayerSaveData.IsValidSetting)
			{
				LoadSaveData(SaveManager->GetPlayerSaveInfo());
			}
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerPawn::Turn);

	PlayerInputComponent->BindAxis(TEXT("MouseMoved"), this, &APlayerPawn::MouseMoved);
	
	PlayerInputComponent->BindAction(TEXT("StoreMouseCoords"), IE_Pressed, this, &APlayerPawn::StoreMouseCoords);
	PlayerInputComponent->BindAction(TEXT("StoreMouseCoords"), IE_Released, this, &APlayerPawn::ClearMouseCoords);

	PlayerInputComponent->BindAction(TEXT("ScrollUp"), IE_Pressed, this, &APlayerPawn::ScrollUp);
	PlayerInputComponent->BindAction(TEXT("ScrollDown"), IE_Pressed, this, &APlayerPawn::ScrollDown);

	PlayerInputComponent->BindAction(TEXT("RotatePlacement"), IE_Pressed, this, &APlayerPawn::RotatePlacement);

	PlayerInputComponent->BindAction(TEXT("Confirm"), IE_Pressed, this, &APlayerPawn::StartConfirm);
	PlayerInputComponent->BindAction(TEXT("Confirm"), IE_Released, this, &APlayerPawn::EndConfirm);
	PlayerInputComponent->BindAction(TEXT("Cancel"), IE_Pressed, this, &APlayerPawn::Cancel);

}

void APlayerPawn::RebindNavigationComponents()
{

}

#pragma region Binds
void APlayerPawn::MoveForward(float InAxis)
{
	OnMoveForward.ExecuteIfBound(InAxis);
}

void APlayerPawn::MoveRight(float InAxis)
{
	OnMoveRight.ExecuteIfBound(InAxis);
}

void APlayerPawn::Turn(float InAxis)
{
	OnTurn.ExecuteIfBound(InAxis);
}

void APlayerPawn::MouseMoved(float InAxis)
{
	OnMouseMoved.Broadcast(InAxis);
}

void APlayerPawn::StoreMouseCoords()
{
	OnMouseLocationStored.ExecuteIfBound();
}

void APlayerPawn::ClearMouseCoords()
{
	OnMouseLocationCleared.ExecuteIfBound();
}

void APlayerPawn::ScrollUp()
{
	OnScrollUp.ExecuteIfBound();
}

void APlayerPawn::ScrollDown()
{
	OnScrollDown.ExecuteIfBound();
}

void APlayerPawn::RotatePlacement()
{
	OnRotatePlacement.ExecuteIfBound();
}

void APlayerPawn::StartConfirm()
{
	OnStartConfirmAction.Broadcast(true);
}

void APlayerPawn::EndConfirm()
{
	OnEndConfirmAction.Broadcast();
}

void APlayerPawn::Cancel()
{
	OnCancelAction.Broadcast();
}

#pragma endregion Binds

FPlayerSaveData APlayerPawn::GetSaveData()
{
	FPlayerSaveData NewData(true, GetActorTransform(), SpringArm->GetComponentTransform());
	return NewData;
}

void APlayerPawn::LoadSaveData(const FPlayerSaveData& LoadedData)
{
	SetActorTransform(LoadedData.PlayerTransform);
	SpringArm->SetWorldTransform(LoadedData.CameraTransform);
}
