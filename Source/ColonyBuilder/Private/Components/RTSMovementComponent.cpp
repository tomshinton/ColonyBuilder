// ColonyBuilder Project, personal project by Tom Shinton

#include "RTSMovementComponent.h"


DEFINE_LOG_CATEGORY(MovementLog);

// Sets default values for this component's properties
URTSMovementComponent::URTSMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void URTSMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void URTSMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CDT = DeltaTime;

	RotateCamera();
}

void URTSMovementComponent::RotateCamera()
{
	float CurrYaw = GetOwner()->GetActorRotation().Yaw;
	float NewYaw = UKismetMathLibrary::FInterpTo(CurrYaw, TargetYaw, CDT, 5);

	FRotator NewRot = GetOwner()->GetActorRotation();
	NewRot.Yaw = NewYaw;

	GetOwner()->SetActorRotation(NewRot);
}

void URTSMovementComponent::MoveForwards(float InAxis)
{
	FVector NewXY = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * (MoveSpeed * InAxis));
	NewXY.Z = GetAppropriateZ(NewXY);

	GetOwner()->SetActorLocation(NewXY);
}

void URTSMovementComponent::MoveRight(float InAxis)
{
	FVector NewXY = GetOwner()->GetActorLocation() + (GetOwner()->GetActorRightVector() * (MoveSpeed * InAxis));
	NewXY.Z = GetAppropriateZ(NewXY);

	GetOwner()->SetActorLocation(NewXY);
}

void URTSMovementComponent::Turn(float InAxis)
{
	TargetYaw = GetOwner()->GetActorRotation().Yaw + RotateSpeed*InAxis;
}

float URTSMovementComponent::GetAppropriateZ(FVector InLocation)
{
	AActor* ReferenceActor = nullptr;
	float CurrZ = GetOwner()->GetActorLocation().Z;
	float OutZ = 0;
	TArray<FHitResult> HitRes;
	FCollisionShape SphereSweep = FCollisionShape::MakeSphere(50);

	FCollisionQueryParams TraceParams;
	TraceParams.TraceTag = "GetZ";
	TraceParams.bTraceComplex = true;

	TraceParams.AddIgnoredActor(GetOwner());
	FVector TraceStart = InLocation + FVector(0, 0, 200);
	FVector TraceEnd = InLocation - FVector(0, 0, 100000);

	GetWorld()->SweepMultiByChannel(HitRes, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, SphereSweep);

#if WITH_EDITOR

	GetWorld()->DebugDrawTraceTag = "GetZ";

#endif //WITH_EDITOR

	for (FHitResult result : HitRes)
	{
		ReferenceActor = result.Actor.Get();
		if (result.Actor->ActorHasTag("Floor"))
		{
			OutZ = result.ImpactPoint.Z + HeightOffset;
			break;
		}
	}

	if (ReferenceActor)
	{
		return OutZ;
	}
	else
	{
		return CurrZ;
	}

}

