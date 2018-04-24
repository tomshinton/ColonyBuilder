// Fill out your copyright notice in the Description page of Project Settings.

#include "ColonyWorldGenModule.h"
#include "ModuleManager.h"

IMPLEMENT_MODULE(FColonyWorldGenModule, ColonyEditor);
DEFINE_LOG_CATEGORY(ColonyWorldGenLog)

void FColonyWorldGenModule::StartupModule()
{
	UE_LOG(ColonyWorldGenLog, Log, TEXT("ColonyWorldGen: Module Started"));
}

void FColonyWorldGenModule::ShutdownModule()
{
	UE_LOG(ColonyWorldGenLog, Warning, TEXT("ColonyWorldGen: Module Shutdown"));
}
