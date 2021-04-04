﻿// Copyright 2021 Yevhenii Selivanov.

#include "BomberEditorModule.h"
//---
#include "AttachedMeshCustomization.h"
#include "MorphDataCustomization.h"
#include "SettingsPickerCustomization.h"
#include "SettingsFunctionCustomization.h"
//---
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FBomberEditorModule, BomberEditor);

static const FName PropertyEditorModule = "PropertyEditor";

void FBomberEditorModule::StartupModule()
{
	if (!FModuleManager::Get().IsModuleLoaded(PropertyEditorModule))
	{
		return;
	}

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(PropertyEditorModule);

	// FAttachedMesh property realizes the functionally of the SSocketChooser
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FAttachedMeshCustomization::PropertyClassName,
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FAttachedMeshCustomization::MakeInstance)
		);

	// Is customized to show only selected in-game option
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FSettingsPickerCustomization::PropertyClassName,
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSettingsPickerCustomization::MakeInstance)
		);

	// Allows to choose ufunction
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FSettingsFunctionCustomization::PropertyClassName,
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSettingsFunctionCustomization::MakeInstance)
		);

	// Allows to choose a morph
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FMorphDataCustomization::PropertyClassName,
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMorphDataCustomization::MakeInstance)
		);

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FBomberEditorModule::ShutdownModule()
{
	if (!FModuleManager::Get().IsModuleLoaded(PropertyEditorModule))
	{
		return;
	}

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(PropertyEditorModule);

	PropertyModule.UnregisterCustomPropertyTypeLayout(FAttachedMeshCustomization::PropertyClassName);
	PropertyModule.UnregisterCustomPropertyTypeLayout(FSettingsPickerCustomization::PropertyClassName);
	PropertyModule.UnregisterCustomPropertyTypeLayout(FSettingsFunctionCustomization::PropertyClassName);
	PropertyModule.UnregisterCustomPropertyTypeLayout(FMorphDataCustomization::PropertyClassName);
}
