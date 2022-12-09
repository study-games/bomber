// Copyright (c) Yevhenii Selivanov

#include "Globals/MyInputMappingContext.h"
//---
#include "EnhancedInputModule.h"
#include "UObject/ObjectSaveContext.h"
//---
#include "Globals/MyInputAction.h"
#include "Globals/PlayerInputDataAsset.h"
//---
#if WITH_EDITOR
#include "EditorUtilsLibrary.h"
#endif

// Returns all input actions set in mappings
void UMyInputMappingContext::GetInputActions(TArray<UMyInputAction*>& OutInputActions) const
{
	for (const FEnhancedActionKeyMapping& MappingIt : Mappings)
	{
		const UMyInputAction* MyInputAction = Cast<UMyInputAction>(MappingIt.Action);
		if (MyInputAction)
		{
			OutInputActions.AddUnique(const_cast<UMyInputAction*>(MyInputAction));
		}
	}
}

// Returns mappings by specified input action
void UMyInputMappingContext::GetMappingsByInputAction(TArray<FEnhancedActionKeyMapping>& OutMappings, const UMyInputAction* InputAction) const
{
	TArray<FEnhancedActionKeyMapping> AllMappings;
	for (const FEnhancedActionKeyMapping& MappingIt : AllMappings)
	{
		if (MappingIt.Action == InputAction)
		{
			OutMappings.Emplace(MappingIt);
		}
	}
}

// Returns all mappings where bIsPlayerMappable is true
void UMyInputMappingContext::GetAllMappings(TArray<FEnhancedActionKeyMapping>& OutMappableData) const
{
	for (const FEnhancedActionKeyMapping& MappingIt : Mappings)
	{
		if (MappingIt.bIsPlayerMappable)
		{
			OutMappableData.Emplace(MappingIt);
		}
	}
}

// Unmap previous key and map new one
bool UMyInputMappingContext::RemapKey(const UInputAction* InInputAction, const FKey& NewKey, const FKey& PrevKey)
{
	if (!ensureMsgf(InInputAction, TEXT("ASSERT: 'InInputAction' is not valid"))
	    || NewKey == PrevKey
	    || UPlayerInputDataAsset::Get().IsMappedKey(NewKey))
	{
		return false;
	}

	bool bRemapped = false;
	for (FEnhancedActionKeyMapping& MappingIt : Mappings)
	{
		if (MappingIt.Action == InInputAction
		    && MappingIt.Key == PrevKey)
		{
			MappingIt.Key = NewKey;
			bRemapped = true;
			break;
		}
	}

	if (!bRemapped)
	{
		return false;
	}

	UEnhancedInputLibrary::RequestRebuildControlMappingsUsingContext(this);

	SaveConfig();

	return true;
}

// Unmap previous key and map new one
bool UMyInputMappingContext::RemapKey(const UMyInputMappingContext* InContext, const FEnhancedActionKeyMapping& InMapping, const FKey& NewKey)
{
	if (!ensureMsgf(InContext, TEXT("%s: 'InContext' is not valid"), *FString(__FUNCTION__)))
	{
		return false;
	}

	UMyInputMappingContext* ContextToRemap = const_cast<UMyInputMappingContext*>(InContext);
	checkf(ContextToRemap, TEXT("%s: 'ContextToRemap' is null"), *FString(__FUNCTION__));
	return ContextToRemap->RemapKey(InMapping.Action, NewKey, InMapping.Key);
}

#if WITH_EDITOR //[IsEditorNotPieWorld]
/** Implemented to save input configs as well. */
void UMyInputMappingContext::PreSave(FObjectPreSaveContext SaveContext)
{
	Super::PreSave(SaveContext);

	// Save only if [IsEditorNotPieWorld]
	if (!UEditorUtilsLibrary::IsEditorNotPieWorld())
	{
		SaveConfig();
	}
}
#endif // WITH_EDITOR [IsEditorNotPieWorld]
