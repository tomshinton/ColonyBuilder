// ColonyBuilder Project, personal project by Tom Shinton

#include "Service_SetSignificantLocationKey.h"
#include "VillagerController.h"
#include "BaseVillager.h"
#include "GarrisonPoint.h"
#include "Utils/Libraries/ManagerUtils.h"
#include "ConstructionManager.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
#include "BuildableBase.h"
#include "BTNodeUtils.h"

DEFINE_LOG_CATEGORY_STATIC(SignificantLocationLog, Log, All);

UService_SetSignificantLocationKey::UService_SetSignificantLocationKey()
{
	NodeName = "Set Significant Key";
}

FString UService_SetSignificantLocationKey::GetStaticDescription() const
{
	FString State;

	switch (Location)
	{
	case ESignificantLocation::Residence:
		State = "Residence";
		break;
	case ESignificantLocation::Workplace:
		State = "Workplace";
		break;
	}
	
	const FString Description = "Go back to " + State;

	return Description;
}

void UService_SetSignificantLocationKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* OwningBlackboard = nullptr;
	ABaseVillager* OwningPawn = nullptr;
	AVillagerController* OwningVillagerController = Cast<AVillagerController>(OwnerComp.GetAIOwner());

	if(OwningVillagerController)
	{
		if (OwningVillagerController->GetBlackboardComponent())
		{
			OwningBlackboard = OwningVillagerController->GetBlackboardComponent();
		}

		OwningPawn = Cast<ABaseVillager>(OwningVillagerController->GetPawn());
	}
	else
	{
		UE_LOG(SignificantLocationLog, Warning, TEXT("Could not cast OwningController to VillagerController"));

	}

	if (!OwningBlackboard)
	{
		UE_LOG(SignificantLocationLog, Warning, TEXT("Could not find OwningBlackboard"));
	}

	FGuid TargetBuildingID;
	switch(Location)
	{
	case ESignificantLocation::Residence:
		TargetBuildingID = OwningPawn->ResidenceID;

	break;
	case ESignificantLocation::Workplace:
		TargetBuildingID = OwningPawn->WorkplaceID;
	break;
	}

	if (TargetBuildingID.IsValid())
	{
		if (UConstructionManager* ConstructionManger = GetManager<UConstructionManager>(GetWorld()))
		{
			TWeakObjectPtr<ABuildableBase> BuildingFromID = ConstructionManger->GetBuildingFromId(TargetBuildingID);
			
			if(BuildingFromID.IsValid())
			{
				ABuildableBase* BuildingFromIdPtr = BuildingFromID.Get();
				if (UGarrisonPoint* CachedGarrisonPoint = BuildingFromIdPtr->GetGarrisonPoint().Get())
				{
					const FVector TargetLocation = GetRandomNavigablePointInVolume(CachedGarrisonPoint->GetComponentLocation(), CachedGarrisonPoint->OverlapComponent->GetScaledBoxExtent(), this);
					OwningBlackboard->SetValue<UBlackboardKeyType_Vector>(KeyName, TargetLocation);

					CachedGarrisonPoint->AddExpectedPawn(OwningPawn);
				}
			}
			else
			{
				ReportServiceError(OwningPawn);
			}
		}
	}
}


void UService_SetSignificantLocationKey::ReportServiceError(ABaseVillager* OwningVillager) const
{
	FString SignificantLocationName;

	switch (Location)
	{
	case ESignificantLocation::Residence:
		SignificantLocationName = "Residence";
		break;
	case ESignificantLocation::Workplace:
		SignificantLocationName = "Workplace";
		break;
	}

	const FString Callback = "Could not find a valid " + SignificantLocationName + " for " + OwningVillager->GetName();
	UE_LOG(SignificantLocationLog, Error, TEXT("%s"), *Callback);
}
