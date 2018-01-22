// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "RTSPlayerController.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Root Component"));
	PawnRoot->SetCollisionProfileName("OverlapAllDynamic");
	RootComponent = PawnRoot;
	

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(PawnRoot);
	SpringArm->SetRelativeRotation(FRotator(-45, 0, 0));
	SpringArm->TargetArmLength = 5000;
	SpringArm->bDoCollisionTest = false;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	PlayerCamera->SetupAttachment(SpringArm);

	MovementComp = CreateDefaultSubobject<URTSMovementComponent>(TEXT("RTS Movement Component"));
	MovementComp->MoveSpeed = 35;
	MovementComp->RotateSpeed = 12;
	MovementComp->HeightOffset = PawnRoot->GetScaledSphereRadius();

	MovementComp->CameraArm = SpringArm;

	BuildComponent = CreateDefaultSubobject<UBuildComponent>(TEXT("Build Component"));

#if WITH_EDITOR

	PawnRoot->bHiddenInGame = false;


#endif //WITH_EDITOR
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	RTSController = Cast<ARTSPlayerController>(GetController());
	
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

}

#pragma region Binds
void APlayerPawn::MoveForward(float InAxis)
{
	MovementComp->MoveForwards(InAxis);
}

void APlayerPawn::MoveRight(float InAxis)
{
	MovementComp->MoveRight(InAxis);
}

void APlayerPawn::Turn(float InAxis)
{
	if (!BuildComponent->GetEnabled())
	{
		MovementComp->Turn(InAxis);
	}
}

void APlayerPawn::MouseMoved(float InAxis)
{
	MovementComp->MouseMoved(InAxis);

	if (RTSController)
	{
		RTSController->UpdateMousePositions();
	}
}

void APlayerPawn::StoreMouseCoords()
{
	MovementComp->StoreMouseCoords();
}

void APlayerPawn::ClearMouseCoords()
{
	MovementComp->ClearMouseCoords();
}

void APlayerPawn::ScrollUp()
{
	if (!BuildComponent->GetEnabled())
	{
		MovementComp->ZoomOut();
	}
}

void APlayerPawn::ScrollDown()
{
	if (!BuildComponent->GetEnabled())
	{
		MovementComp->ZoomIn();
	}
}

#pragma endregion Binds