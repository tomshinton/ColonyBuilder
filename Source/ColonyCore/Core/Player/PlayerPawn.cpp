// ColonyBuilder Project, personal project by Tom Shinton

#include "PlayerPawn.h"

#include "Kismet/GameplayStatics.h"
#include "Utils/Libraries/ManagerUtils.h"
#include <Components/SphereComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "../Components/MoveComponent.h"
#include "BuildSystem/BuildComponent/BuildComponent.h"
#include "../Selection/SelectionComponent.h"

APlayerPawn::APlayerPawn()
	: PawnRoot(CreateDefaultSubobject<USphereComponent>(TEXT("PlayerPawnRoot")))
	, SpringArm(CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm")))
	, PlayerCamera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component")))
	, MovementComp(CreateDefaultSubobject<UMoveComponent>(TEXT("RTS Movement Component")))
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

	if (USaveManager* SaveManager = GetManager<USaveManager>(this))
	{
		LoadSaveData(SaveManager->GetCachedPlayerData());
	}
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), MovementComp, &UMoveComponent::MoveForwards);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), MovementComp, &UMoveComponent::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), MovementComp, &UMoveComponent::Turn);
	PlayerInputComponent->BindAxis(TEXT("MouseMoved"), this, &APlayerPawn::MouseMoved);

	PlayerInputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, MovementComp, &UMoveComponent::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, MovementComp, &UMoveComponent::ZoomOut);

	
	PlayerInputComponent->BindAction(TEXT("StoreMouseCoords"), IE_Pressed, this, &APlayerPawn::StoreMouseCoords);
	PlayerInputComponent->BindAction(TEXT("ClearMouseCoords"), IE_Released, this, &APlayerPawn::ClearMouseCoords);
	   
	//Building
	if (BuildComponent)
	{
		PlayerInputComponent->BindAction(TEXT("RotatePlacement"), IE_Pressed, BuildComponent, &UBuildComponent::RotatePlacement);
		PlayerInputComponent->BindAction(TEXT("StartBuild"), IE_Pressed, BuildComponent, &UBuildComponent::StartBuild);
		PlayerInputComponent->BindAction(TEXT("EndBuild"), IE_Released, BuildComponent, &UBuildComponent::EndBuild);
		PlayerInputComponent->BindAction(TEXT("CancelBuild"), IE_Pressed, BuildComponent, &UBuildComponent::CancelBuild);
	}
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
