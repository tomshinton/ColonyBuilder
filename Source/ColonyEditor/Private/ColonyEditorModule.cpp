// Fill out your copyright notice in the Description page of Project Settings.

#include "ModuleManager.h"
#include "ColonyEditorModule.h"

IMPLEMENT_MODULE(FColonyEditorModule, ColonyEditor);
DEFINE_LOG_CATEGORY(ColonyEditorLog)

void FColonyEditorModule::StartupModule()
{
	UE_LOG(ColonyEditorLog, Log, TEXT("ColonyEditor: Module Started"));
}

void FColonyEditorModule::ShutdownModule()
{
	UE_LOG(ColonyEditorLog, Warning, TEXT("ColonyEditor: Module Shutdown"));
}
