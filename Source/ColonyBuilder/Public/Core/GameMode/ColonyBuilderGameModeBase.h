// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PlayerPawn.h"

#include "GameFramework/GameModeBase.h"
#include "ColonyBuilderGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COLONYBUILDER_API AColonyBuilderGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AColonyBuilderGameModeBase();

public:
	static const int32 GridSize;
	static const bool SaveOnExit;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
