// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_Monologue.generated.h"

namespace MonologuePinNameHelper {
	const FName DataTablePin    = "DialogueList";
	const FName RowNamePin      = "RowName";
	const FName InteractionPin  = "Interaction";
	// const FName InteractionOutputPin = "Interaction";
	const FName PreviousNodePin = "PreviousNode";
	const FName LinesPin = "Lines";
	const FName DelayPin = "DelayBetweenLines";
	const FName SpeakerPin    = "Speaker";
	const FName WaitBeforePin = "WaitBefore";
	const FName ActivePin     = "Active";
	const FName BlockInputPin = "Block Input";
	// const FName AddSpecItemsToInventoryPin = "AddSpecItemsToInventory";
	const FName ThisNodePin   = "ThisNode";
}

/**
 * 
 */
UCLASS()
class MUKA_API UK2Node_Monologue : public UK2Node
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphNode Interface.
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FText GetTooltipText() const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void PostReconstructNode() override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
	virtual void PreloadRequiredAssets() override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	//~ End UK2Node Interface

	// /** Get the return type of our struct */
	UScriptStruct* GetReturnTypeForStruct();

	// /** Get the then output pin */
	UEdGraphPin* GetThenPin() const;
	/** Get the Data Table input pin */
	UEdGraphPin* GetDataTablePin(const TArray<UEdGraphPin*>* InPinsToSearch=NULL) const;
	/** Get the spawn transform input pin */	
	UEdGraphPin* GetRowNamePin() const;
	UEdGraphPin* GetLinesPin() const;
	// UEdGraphPin* GetInteractionPin() const;
	// UEdGraphPin* GetPreviousNodePin() const;
	// UEdGraphPin* GetThisNodePin() const;
	// UEdGraphPin* GetDelayPin() const;
	// UEdGraphPin* GetSpeaker() const;
	// UEdGraphPin* GetWaitBefore() const;
	// UEdGraphPin* GetActivePin() const;
	// UEdGraphPin* InteractionOutputPin() const;
	
    // /** Get the exec output pin for when the row was not found */
	// UEdGraphPin* GetRowNotFoundPin() const;

	/** Get the result output pin */
	UEdGraphPin* GetResultPin() const;

	// /** Get the type of the TableRow to return */
	UScriptStruct* GetDataTableRowStructType() const;

	void OnDataTableRowListChanged(const UDataTable* DataTable);
private:
	/**
	 * Takes the specified "MutatablePin" and sets its 'PinToolTip' field (according
	 * to the specified description)
	 * 
	 * @param   MutatablePin	The pin you want to set tool-tip text on
	 * @param   PinDescription	A string describing the pin's purpose
	 */
	void SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const;

	/** Set the return type of our struct */
	// void SetReturnTypeForStruct(UScriptStruct* InClass);
	/** Queries for the authoritative return type, then modifies the return pin to match */
	// void RefreshOutputPinType();
	/** Triggers a refresh which will update the node's widget; aimed at updating the dropdown menu for the RowName input */
	void RefreshRowNameOptions();

	void RefreshFromDataTableRow(FName RowName);

	/** Tooltip text for this node. */
	FText NodeTooltip;

	/** Constructing FText strings can be costly, so we cache the node's title */
	FNodeTextCache CachedNodeTitle;
	
};
