// ColonyBuilder Project, personal project by Tom Shinton

#include "GarrisonPoint.h"
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>

DEFINE_LOG_CATEGORY_STATIC(GarrisonComponentLog, Log, All);

UGarrisonPoint::UGarrisonPoint()
	: MeshComponent(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")))
	, OverlapComponent(CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent")))
{
	PrimaryComponentTick.bCanEverTick = false;

	MeshComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OverlapComponent->SetupAttachment(this);
	OverlapComponent->SetCollisionProfileName("OverlapAllDynamic");

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &UGarrisonPoint::AttemptGarrison);
}

void UGarrisonPoint::AttemptGarrison(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(GarrisonComponentLog, Log, TEXT("Attempting to garrison %s in %s"), *OtherActor->GetName(), *GetOwner()->GetName());

	if (ABaseVillager* OtherActorAsVillager = Cast<ABaseVillager>(OtherActor))
	{
		if (ExpectedGarrisonedVillagers.Contains(OtherActorAsVillager))
		{
			GarrisonedVillagers.AddUnique(OtherActorAsVillager);
			OtherActor->SetActorHiddenInGame(true);
			ExpectedGarrisonedVillagers.Remove(OtherActorAsVillager);
		}
	}
}

void UGarrisonPoint::Ungarrison(ABaseVillager* BaseVillager)
{
	if (GarrisonedVillagers.Contains(BaseVillager))
	{
		GarrisonedVillagers.Remove(BaseVillager);
		BaseVillager->SetActorHiddenInGame(false);
	}
}
