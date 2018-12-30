// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_SetSignificantLocationKey.generated.h"

class ABaseVillager;

//////////////////////////////////////////////////////////////////////////
// Service to set MoveToLocation key to a significant location
//////////////////////////////////////////////////////////////////////////

UENUM()
enum class ESignificantLocation : uint8
{
	Workplace,
	Residence
};

UCLASS()
class COLONYAI_API UService_SetSignificantLocationKey : public UBTService
{
	GENERATED_BODY()

public:

	UService_SetSignificantLocationKey();

	virtual FString GetStaticDescription() const override;
	
	UPROPERTY(EditAnywhere, Category = Key)
	FName KeyName;

	UPROPERTY(EditAnywhere, Category = Location)
	ESignificantLocation Location;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void ReportServiceError(ABaseVillager* OwningVillager) const;
};
