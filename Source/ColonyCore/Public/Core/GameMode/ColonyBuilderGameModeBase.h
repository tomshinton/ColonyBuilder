// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class APlayerPawn;
class ABaseVillager;

#include <GameFramework/GameModeBase.h>

#include "ColonyBuilderGameModeBase.generated.h"

UCLASS()
class COLONYCORE_API AColonyBuilderGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AColonyBuilderGameModeBase();

public:
	UPROPERTY(EditDefaultsOnly, Category = Defaults)
	TSubclassOf<ABaseVillager> VillagerClass;

	static const bool SaveOnExit;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void BeginPlay() override;
};
