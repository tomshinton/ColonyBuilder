// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "ColonyManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ColonyManagerLog, Log, All);

UCLASS()
class COLONYCORE_API UColonyManager : public UObject
{
	GENERATED_BODY()

public:	
	virtual void Init(const TFunction<void()> InitCallback);
	virtual class UWorld* GetWorld() const override;

	FString GetManagerName() const { return ManagerName; };

protected:

	FString ManagerName;

private:

	TFunction<void()> InitFinishedCallback;
};
