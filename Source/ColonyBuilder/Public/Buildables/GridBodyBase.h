// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BuildableBase.h"

#include "GridBodyBase.generated.h"

UCLASS()
class COLONYBUILDER_API AGridBodyBase : public AActor, public ISavableInterface
{
	GENERATED_BODY()

	AGridBodyBase();

public:

	static const FName MeshName;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")	
	USceneComponent* SceneRoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meshes")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meshes")
	UBoxComponent* GridBodyCollision;
	
private:
	int32 BuildingID;

	//ISavableInterface
	virtual FBuildingSaveData GetBuildingSaveData() override;
	virtual void LoadBuildingSaveData(FBuildingSaveData LoadedData) override;
	//ISavableInterface


};
