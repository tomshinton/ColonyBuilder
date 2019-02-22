// ColonyBuilder Project, personal project by Tom Shinton

#include "PlayerPawn.h"
#include "RTSPlayerController.h"

#include "SaveManager.h"
#include "ColonyInstance.h"
#include "ColonySave.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/Libraries/ManagerUtils.h"

APlayerPawn::APlayerPawn()
	: PawnRoot(CreateDefaultSubobject<USphereComponent>(TEXT("PlayerPawnRoot")))
	, SpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm")))
	, PlayerCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component")))
	, MovementComp(CreateDefaultSubobject<URTSMovementComponent>(TEXT("RTS Movement Component")))
	, BuildComponent(CreateDefaultSubobject<UBuildComponent>(TEXT("Build Component")))
	, SelectionComponent(CreateDefaultSubobject<USelectionComponent>(TEXT("Selection Component")))
{
	PrimaryActorTick.bCanEverTick = false;

	PawnRoot->SetCollisionProfileName("NoCollision");
	RootComponent = PawnRoot;

	SpringArm->SetupAttachment(PawnRoot);
	SpringArm->SetRelativeRotation(FRotator(-45, 0, 0));
	SpringArm->TargetArmLength = 10000;
	SpringArm->bDoCollisionTest = false;

	PlayerCamera->SetupAttachment(SpringArm);

	MovementComp->MoveSpeed = 35;
	MovementComp->RotateSpeed = 12;
	MovementComp->HeightOffset = PawnRoot->GetScaledSphereRadius();

	MovementComp->CameraArm = SpringArm;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//Load any saved data, if there is any
	if (USaveManager* SaveManager = GetManager<USaveManager>(this))
	{
		LoadSaveData(SaveManager->GetCachedPlayerData());
	}
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
	if (this)
	{
		FPlayerSaveData NewData(true, GetActorTransform(), SpringArm->GetComponentTransform());
		return NewData;
	}
	
	return FPlayerSaveData();
}

void APlayerPawn::LoadSaveData(const FPlayerSaveData& LoadedData)
{
	if (LoadedData.IsValidSetting)
	{
		SetActorTransform(LoadedData.PlayerTransform);
		SpringArm->SetWorldTransform(LoadedData.CameraTransform);
	}
}
