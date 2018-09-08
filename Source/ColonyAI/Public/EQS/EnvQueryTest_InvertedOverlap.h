// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_InvertedOverlap.generated.h"

class AActor;
struct FCollisionQueryParams;
struct FCollisionShape;

UCLASS()
class COLONYAI_API UEnvQueryTest_InvertedOverlap : public UEnvQueryTest
{
	GENERATED_BODY()

	UEnvQueryTest_InvertedOverlap();

public:
	UPROPERTY(EditDefaultsOnly, Category = Overlap)
	TArray<FEnvOverlapData> OverlapData;
	
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

protected:

	DECLARE_DELEGATE_RetVal_SixParams(bool, FRunOverlapSignatureInverted, const FVector&, const FCollisionShape&, AActor*, UWorld*, enum ECollisionChannel, const FCollisionQueryParams&);

	bool RunOverlap(const FVector& ItemPos, const FCollisionShape& CollisionShape, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params);
	bool RunOverlapBlocking(const FVector& ItemPos, const FCollisionShape& CollisionShape, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params);
};
