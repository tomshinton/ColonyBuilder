// ColonyBuilder Project, personal project by Tom Shinton

#include "GhostFF.h"
#include "BuildableBase.h"
#include <Components/SceneComponent.h>

#include "ValidationRunner.h"
#include <Engine/Engine.h>
#include "ColonyGridSettings.h"

DEFINE_LOG_CATEGORY(GhostFFLog)

AGhostFF::AGhostFF()
	: DuplicatedObjects()
	, GhostBounds(FVector(ForceInitToZero))
	, GhostOrigin(FVector(ForceInitToZero))
{
}

void AGhostFF::GhostStart()
{
	check(BuildingData);
	
	ABuildableBase* BuildingToRepresent = Cast<ABuildableBase>(BuildingData->BuildingClass->GetDefaultObject(true));

	if (UWorld* World = GetWorld())
	{
		const FActorSpawnParameters SpawnParams;
		ABuildableBase* BuildingToRepresent = World->SpawnActor<ABuildableBase>(BuildingData->BuildingClass, FVector(ForceInitToZero), FRotator(ForceInitToZero), SpawnParams);
		if (BuildingToRepresent)
		{
			BuildingToRepresent->SetActorHiddenInGame(true);
			TArray<USceneComponent*> DefaultSubObjects;
			BuildingToRepresent->GetComponents<USceneComponent>(DefaultSubObjects, false);

			if (DefaultSubObjects.Num() > 0)
			{
				DuplicatedObjects.Reserve(DefaultSubObjects.Num());
				for (const USceneComponent* DefaultSubObject : DefaultSubObjects)
				{
					FString NewName = *DefaultSubObject->GetName();
					NewName += "_Duplicated";
					if (USceneComponent* DupedComponent = DuplicateObject(DefaultSubObject, this, FName(*NewName)))
					{
						if (!DupedComponent->IsTemplate())
						{
							if (UPrimitiveComponent* CompAsPrim = Cast<UPrimitiveComponent>(DupedComponent))
							{
								CompAsPrim->SetCollisionProfileName("Ghost");
							}
							const FTransform ExistingTransform = DefaultSubObject->GetRelativeTransform();

							DuplicatedObjects.Add(DupedComponent);
							DupedComponent->RegisterComponent();
							DupedComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true));
						}
					}
				}
			}

			BuildingToRepresent->Destroy();
		}
	}
}

void AGhostFF::BuildTestPoints()
{
	if (UWorld* World = GetWorld())
	{
		const FVector ActorLoc = GetActorLocation();

		if (DuplicatedObjects.Num() > 0)
		{
			GetActorBounds(false, GhostOrigin, GhostBounds);
		}

		TArray<FSubBuilding> TestPoints;

		if (UColonyGridSettings* GridSettings = GetMutableDefault<UColonyGridSettings>())
		{
			const int32 GridSize = GridSettings->GridSize;
			const int32 SquareTestPoints = FMath::CeilToInt(FMath::Max(GhostBounds.X, GhostBounds.Y) / GridSize) + 1; //Add some small padding - we can prune these later

			TestPoints.Reserve(FMath::Square(SquareTestPoints));

			for (int32 x = -SquareTestPoints; x < SquareTestPoints; x++)
			{
				for (int32 y = -SquareTestPoints; y < SquareTestPoints; y++)
				{
					const FVector NewPoint = ActorLoc + FVector(x * GridSize, y * GridSize, 20.f);
					TestPoints.Emplace(NewPoint, EPointType::BuildingPoint);
				}
			}
		}

		if (TestPoints.Num() > 0)
		{
			ValidationRunner->Validate(*World, StaticClass(), TestPoints, *BuildingData);
			ValidationRunner->OnValidationComplete.BindLambda([&](const bool IsValidPlacement, const TArray<FSubBuilding>& ValidatedPoints)
			{

			});
		}
	}
}
