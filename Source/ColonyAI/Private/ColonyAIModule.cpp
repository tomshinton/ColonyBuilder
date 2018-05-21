// Fill out your copyright notice in the Description page of Project Settings.

#include "ColonyAIModule.h"
#include "ModuleManager.h"

IMPLEMENT_GAME_MODULE(FColonyAIModule, ColonyAI);
DEFINE_LOG_CATEGORY(ColonyAILog)

void FColonyAIModule::StartupModule()
{
	UE_LOG(ColonyAILog, Log, TEXT("ColonyAI: Module Started"));
}

void FColonyAIModule::ShutdownModule()
{
	UE_LOG(ColonyAILog, Warning, TEXT("ColonyAI: Module Shutdown"));
}
