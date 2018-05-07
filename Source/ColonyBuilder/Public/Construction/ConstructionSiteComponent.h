// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "ConstructionSiteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLONYBUILDER_API UConstructionSiteComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UConstructionSiteComponent();

	UBoxComponent* CollisionComponent;

	UBillboardComponent* Sprite;

	int32 GetNumBuilders() { return UniqueBuilders.Num(); }

private:
	UFUNCTION()
	void OnEnterConstructionSite(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnLeaveConstructionSite(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TArray<AActor*> UniqueBuilders;
};
