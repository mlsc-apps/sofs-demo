// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_AddInteraction.h"
#include "Engine/DataTable.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
// #include "K2Node_IfThenElse.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "DataTableEditorUtils.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "../BlueprintFunctionLibrary/MUKABlueprintFunctionLibrary.h"
#include "../Interactable.h"
#include "../Interaction.h"
#include "BlueprintActionDatabaseRegistrar.h"

#define LOCTEXT_NAMESPACE "K2Node_AddInteraction"

UK2Node_AddInteraction::UK2Node_AddInteraction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Adds new Interaction to Interactable");
}

void UK2Node_AddInteraction::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	UEdGraphPin* ExecutePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* ThenPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	ThenPin->PinFriendlyName = LOCTEXT("InteractionAddedPin Row Found Exec pin", "");
	// CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetDataTableRowHelper::RowNotFoundPinName);

	// UEdGraphNode::FCreatePinParams InteractablePinParams;
	// InteractablePinParams.bIsReference = true;
	UEdGraphPin* InteractablePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteractable::StaticClass(), GetDataTableRowHelper::InteractablePinName);

	// Add DataTable pin
	UEdGraphPin* DataTablePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UDataTable::StaticClass(), GetDataTableRowHelper::DataTablePinName);
	SetPinToolTip(*DataTablePin, LOCTEXT("DataTablePinDescription", "The DataTable you want to retreive a row from"));

	// Row Name pin
	UEdGraphPin* RowNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, GetDataTableRowHelper::RowNamePinName);
	SetPinToolTip(*RowNamePin, LOCTEXT("RowNamePinDescription", "The name of the row to retrieve from the DataTable"));

	// Interaction Pins
	UEdGraphPin* NamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, GetDataTableRowHelper::NamePinName);
	UEdGraphPin* ActionNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, GetDataTableRowHelper::ActionPinName);
	UEdGraphPin* WhenInventoryItemsPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, GetDataTableRowHelper::WhenInventoryItemsPinName);
	// UEdGraphPin* OutputInventoryItemsNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, GetDataTableRowHelper::OutputInventoryItemsPinName);

	UEnum* const FinalizeWithEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EFinalizeWith"), true);
	UEdGraphPin* FinalizeWith = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Byte, FinalizeWithEnum, GetDataTableRowHelper::FinalizeWithPinName);
	FinalizeWith->DefaultValue = FinalizeWithEnum->GetNameStringByValue(static_cast<int>(EFinalizeWith::Reset));
	// EFinalizeWith FinalizeWithValue = EFinalizeWith::Reset;
	// FinalizeWith->DefaultValue = "Reset"; //FinalizeWithValue; // == EFinalizeWith::Reset ? "Reset" : "Destroy";

	// Boolean Standard Pins
	UEdGraphPin* RunOnPlayPinName = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, GetDataTableRowHelper::RunOnPlayPinName);
	RunOnPlayPinName->DefaultValue = "false";
	UEdGraphPin* ShowablePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, GetDataTableRowHelper::ShowablePinName);
	ShowablePin->DefaultValue = "true";
	UEdGraphPin* AutoPinName = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, GetDataTableRowHelper::AutoPinName);
	AutoPinName->DefaultValue = "false";

	// Result pin
	// UEdGraphNode::FCreatePinParams InteractionPinParams;
	// InteractionPinParams.bIsReference = true;
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, UInteraction::StaticClass(), UEdGraphSchema_K2::PN_ReturnValue);
	ResultPin->PinFriendlyName = LOCTEXT("Interaction", "Interaction");
	// SetPinToolTip(*ResultPin, LOCTEXT("ResultPinDescription", "The returned TableRow, if found"));

	Super::AllocateDefaultPins();
}

void UK2Node_AddInteraction::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
{
	MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

	UEdGraphSchema_K2 const* const K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema());
	if (K2Schema != nullptr)
	{
		MutatablePin.PinToolTip += TEXT(" ");
		MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
	}

	MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

// void UK2Node_AddInteraction::RefreshOutputPinType()
// {
// 	UScriptStruct* OutputType = GetDataTableRowStructType();
// 	SetReturnTypeForStruct(OutputType);
// }

void UK2Node_AddInteraction::RefreshRowNameOptions()
{
	// When the DataTable pin gets a new value assigned, we need to update the Slate UI so that SGraphNodeCallParameterCollectionFunction will update the ParameterName drop down
	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}


// void UK2Node_AddInteraction::SetReturnTypeForStruct(UScriptStruct* NewRowStruct)
// {
// 	UScriptStruct* OldRowStruct = GetReturnTypeForStruct();
// 	if (NewRowStruct != OldRowStruct)
// 	{
// 		UEdGraphPin* ResultPin = GetResultPin();

// 		if (ResultPin->SubPins.Num() > 0)
// 		{
// 			GetSchema()->RecombinePin(ResultPin);
// 		}

// 		// NOTE: purposefully not disconnecting the ResultPin (even though it changed type)... we want the user to see the old
// 		//       connections, and incompatible connections will produce an error (plus, some super-struct connections may still be valid)
// 		ResultPin->PinType.PinSubCategoryObject = NewRowStruct;
// 		ResultPin->PinType.PinCategory = (NewRowStruct == nullptr) ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;

// 		CachedNodeTitle.Clear();
// 	}
// }

UScriptStruct* UK2Node_AddInteraction::GetReturnTypeForStruct()
{
	UScriptStruct* ReturnStructType = (UScriptStruct*)(GetResultPin()->PinType.PinSubCategoryObject.Get());

	return ReturnStructType;
}

UScriptStruct* UK2Node_AddInteraction::GetDataTableRowStructType() const
{
	UScriptStruct* RowStructType = nullptr;

	UEdGraphPin* DataTablePin = GetDataTablePin();
	if(DataTablePin && DataTablePin->DefaultObject != nullptr && DataTablePin->LinkedTo.Num() == 0)
	{
		if (const UDataTable* DataTable = Cast<const UDataTable>(DataTablePin->DefaultObject))
		{
			RowStructType = DataTable->RowStruct;
		}
	}

	if (RowStructType == nullptr)
	{
		UEdGraphPin* ResultPin = GetResultPin();
		if (ResultPin && ResultPin->LinkedTo.Num() > 0)
		{
			RowStructType = Cast<UScriptStruct>(ResultPin->LinkedTo[0]->PinType.PinSubCategoryObject.Get());
			for (int32 LinkIndex = 1; LinkIndex < ResultPin->LinkedTo.Num(); ++LinkIndex)
			{
				UEdGraphPin* Link = ResultPin->LinkedTo[LinkIndex];
				UScriptStruct* LinkType = Cast<UScriptStruct>(Link->PinType.PinSubCategoryObject.Get());

				if (RowStructType->IsChildOf(LinkType))
				{
					RowStructType = LinkType;
				}
			}
		}
	}
	return RowStructType;
}

void UK2Node_AddInteraction::OnDataTableRowListChanged(const UDataTable* DataTable)
{
	UEdGraphPin* DataTablePin = GetDataTablePin();
	if (DataTable && DataTablePin && DataTable == DataTablePin->DefaultObject)
	{
		UEdGraphPin* RowNamePin = GetRowNamePin();
		const bool TryRefresh = RowNamePin && !RowNamePin->LinkedTo.Num();
		const FName CurrentName = RowNamePin ? FName(*RowNamePin->GetDefaultAsString()) : NAME_None;
		if (TryRefresh && RowNamePin && !DataTable->GetRowNames().Contains(CurrentName))
		{
			if (UBlueprint* BP = GetBlueprint())
			{
				FBlueprintEditorUtils::MarkBlueprintAsModified(BP);
			}
		}
	}
}

void UK2Node_AddInteraction::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) 
{
	Super::ReallocatePinsDuringReconstruction(OldPins);

	if (UEdGraphPin* DataTablePin = GetDataTablePin(&OldPins))
	{
		if (UDataTable* DataTable = Cast<UDataTable>(DataTablePin->DefaultObject))
		{
			// make sure to properly load the data-table object so that we can 
			// farm the "RowStruct" property from it (below, in GetDataTableRowStructType)
			PreloadObject(DataTable);
		}
	}
}

void UK2Node_AddInteraction::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

// FText UK2Node_AddInteraction::GetMenuCategory() const
// {
// 	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
// }

bool UK2Node_AddInteraction::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const
{
	if (MyPin == GetResultPin() && MyPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
	{
		bool bDisallowed = true;
		if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
		{
			if (UScriptStruct* ConnectionType = Cast<UScriptStruct>(OtherPin->PinType.PinSubCategoryObject.Get()))
			{
				bDisallowed = !FDataTableEditorUtils::IsValidTableStruct(ConnectionType);
			}
		}
		else if (OtherPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
		{
			bDisallowed = false;
		}

		if (bDisallowed)
		{
			OutReason = TEXT("Must be a struct that can be used in a DataTable");
		}
		return bDisallowed;
	}
	return false;
}

void UK2Node_AddInteraction::RefreshFromDataTableRow(FName RowName) {
    FString ContextString;
	UDataTable* Table = Cast<UDataTable>(GetDataTablePin()->DefaultObject);
	if (Table) {
		FMUKAInteractionSpec* RowPtr = Table->FindRow<FMUKAInteractionSpec>(RowName, ContextString);
		if (RowPtr) {
			GetActionNamePin()->DefaultValue = RowPtr->ActionName;
			GetInteractionNamePin()->DefaultValue = RowName.ToString();
			GetWhenInventoryItemsPin()->DefaultValue = RowPtr->WhenInventoryItems;
			// GetOutputInventoryItemsPin()->DefaultValue = RowPtr->AddInventoryItemsWhenFinished;
		}
	}

}

void UK2Node_AddInteraction::PinDefaultValueChanged(UEdGraphPin* ChangedPin) 
{
	if (ChangedPin && ChangedPin->PinName == GetDataTableRowHelper::DataTablePinName)
	{
		// RefreshOutputPinType();

		UEdGraphPin* RowNamePin = GetRowNamePin();
		UDataTable*  DataTable = Cast<UDataTable>(ChangedPin->DefaultObject);
		if (RowNamePin)
		{
			if (DataTable && (RowNamePin->DefaultValue.IsEmpty() || !DataTable->GetRowMap().Contains(*RowNamePin->DefaultValue))) {
				if (auto Iterator = DataTable->GetRowMap().CreateConstIterator()) {
					RowNamePin->DefaultValue = Iterator.Key().ToString();
				}
				RefreshFromDataTableRow(FName(GetRowNamePin()->DefaultValue));
			}	

			RefreshRowNameOptions();
		}
	}

	if (ChangedPin && ChangedPin->PinName == GetDataTableRowHelper::RowNamePinName) {
		RefreshFromDataTableRow(FName(ChangedPin->DefaultValue));
	}

}

FText UK2Node_AddInteraction::GetTooltipText() const
{
	return NodeTooltip;
}

UEdGraphPin* UK2Node_AddInteraction::GetThenPin()const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_AddInteraction::GetDataTablePin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;
    
	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* TestPin : *PinsToSearch)
	{
		if (TestPin && TestPin->PinName == GetDataTableRowHelper::DataTablePinName)
		{
			Pin = TestPin;
			break;
		}
	}
	check(Pin == nullptr || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_AddInteraction::GetRowNamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::RowNamePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_AddInteraction::GetActionNamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::ActionPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_AddInteraction::GetInteractionNamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::NamePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_AddInteraction::GetWhenInventoryItemsPin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::WhenInventoryItemsPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

// UEdGraphPin* UK2Node_AddInteraction::GetOutputInventoryItemsPin() const
// {
// 	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::OutputInventoryItemsPinName);
// 	check(Pin->Direction == EGPD_Input);
// 	return Pin;
// }

UEdGraphPin* UK2Node_AddInteraction::GetFinalizeWithPin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::FinalizeWithPinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_AddInteraction::GetInteractablePin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::InteractablePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

// Output Pins

UEdGraphPin* UK2Node_AddInteraction::GetRowNotFoundPin() const
{
	UEdGraphPin* Pin = FindPinChecked(GetDataTableRowHelper::RowNotFoundPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_AddInteraction::GetResultPin() const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

FText UK2Node_AddInteraction::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("MUKA_NodeTitle", "Add Interaction");
}

// FText UK2Node_AddInteraction::GetTooltipText() const
// {
//     return LOCTEXT("SOU_Interaction_Tooltip", "This is our Tutorial K2 Node Tooltip information!");
// }

FText UK2Node_AddInteraction::GetMenuCategory() const
{
    return LOCTEXT("MUKA_MenuCategory", "MUKA");
}

// FText UK2Node_AddInteraction::GetNodeTitle(ENodeTitleType::Type TitleType) const
// {
// 	if (TitleType == ENodeTitleType::MenuTitle)
// 	{
// 		return LOCTEXT("ListViewTitle", "Get Data Table Row");
// 	}
// 	else if (UEdGraphPin* DataTablePin = GetDataTablePin())
// 	{
// 		if (DataTablePin->LinkedTo.Num() > 0)
// 		{
// 			return NSLOCTEXT("K2Node", "DataTable_Title_Unknown", "Get Data Table Row");
// 		}
// 		else if (DataTablePin->DefaultObject == nullptr)
// 		{
// 			return NSLOCTEXT("K2Node", "DataTable_Title_None", "Get Data Table Row NONE");
// 		}
// 		else if (CachedNodeTitle.IsOutOfDate(this))
// 		{
// 			FFormatNamedArguments Args;
// 			Args.Add(TEXT("DataTableName"), FText::FromString(DataTablePin->DefaultObject->GetName()));

// 			FText LocFormat = NSLOCTEXT("K2Node", "DataTable", "Get Data Table Row {DataTableName}");
// 			// FText::Format() is slow, so we cache this to save on performance
// 			CachedNodeTitle.SetCachedText(FText::Format(LocFormat, Args), this);
// 		}
// 	}
// 	else
// 	{
// 		return NSLOCTEXT("K2Node", "DataTable_Title_None", "Get Data Table Row NONE");
// 	}	
// 	return CachedNodeTitle;
// }

void UK2Node_AddInteraction::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    Super::ExpandNode(CompilerContext, SourceGraph);
    
    UEdGraphPin* InnerDataTableInPin = GetDataTablePin();
    // UDataTable* Table = (InnerDataTableInPin != NULL) ? Cast<UDataTable>(InnerDataTableInPin->DefaultObject) : NULL;
    // if((nullptr == InnerDataTableInPin) || (0 == InnerDataTableInPin->LinkedTo.Num() && nullptr == Table))
    // {
    //     CompilerContext.MessageLog.Error(*LOCTEXT("GetDataTableRowNoDataTable_Error", "GetDataTableRow must have a DataTable specified.").ToString(), this);
    //     // we break exec links so this is the only error we get
    //     BreakAllNodeLinks();
    //     return;
    // }

	// FUNCTION NODE
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UMUKABlueprintFunctionLibrary, AddInteraction);
	UK2Node_CallFunction* AddInteractionOuter = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	AddInteractionOuter->FunctionReference.SetExternalMember(FunctionName, UMUKABlueprintFunctionLibrary::StaticClass());
	AddInteractionOuter->AllocateDefaultPins();
    CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(AddInteractionOuter->GetExecPin()));

	// Connect the input of our GetDataTableRow to the Input of our Function pin
    UEdGraphPin* OuterActionNamePin      = AddInteractionOuter->FindPinChecked(TEXT("Action"));
	UEdGraphPin* OuterInteractionNamePin = AddInteractionOuter->FindPinChecked(TEXT("Name"));
	UEdGraphPin* OuterInteractablePin    = AddInteractionOuter->FindPinChecked(TEXT("Interactable"));
	UEdGraphPin* OuterRunOnPlay          = AddInteractionOuter->FindPinChecked(TEXT("RunOnPlay"));
	UEdGraphPin* OuterFinalizeWith       = AddInteractionOuter->FindPinChecked(TEXT("FinalizeWith"));
	UEdGraphPin* OuterWhenInventoryItems = AddInteractionOuter->FindPinChecked(TEXT("WhenInventoryItems"));
	UEdGraphPin* OuterAutoNamePin        = AddInteractionOuter->FindPinChecked(TEXT("Auto"));

	UEdGraphPin* InnerReturnValuePin = this->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    UEdGraphPin* OuterReturnValuePin = AddInteractionOuter->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    OuterReturnValuePin->PinType = InnerReturnValuePin->PinType;
	OuterReturnValuePin->PinType.PinSubCategoryObject = InnerReturnValuePin->PinType.PinSubCategoryObject;
	CompilerContext.MovePinLinksToIntermediate(*InnerReturnValuePin, *OuterReturnValuePin);
	CompilerContext.MovePinLinksToIntermediate(*GetInteractablePin(), *OuterInteractablePin);

	CompilerContext.MovePinLinksToIntermediate(*GetActionNamePin(), *OuterActionNamePin);
	CompilerContext.MovePinLinksToIntermediate(*GetInteractionNamePin(), *OuterInteractionNamePin);
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(GetDataTableRowHelper::RunOnPlayPinName), *OuterRunOnPlay);
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(GetDataTableRowHelper::FinalizeWithPinName), *OuterFinalizeWith);
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(GetDataTableRowHelper::WhenInventoryItemsPinName), *OuterWhenInventoryItems);
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(GetDataTableRowHelper::AutoPinName), *OuterAutoNamePin);

    UEdGraphPin* OuterThenPin = AddInteractionOuter->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *OuterThenPin);

	BreakAllNodeLinks();
}

FSlateIcon UK2Node_AddInteraction::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_AddInteraction::PostReconstructNode()
{
	Super::PostReconstructNode();

	// RefreshOutputPinType();
}

void UK2Node_AddInteraction::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);

	const UEdGraphPin* DataTablePin = GetDataTablePin();
	const UEdGraphPin* RowNamePin = GetRowNamePin();
	if (!DataTablePin || !RowNamePin)
	{
		MessageLog.Error(*LOCTEXT("MissingPins", "Missing pins in @@").ToString(), this);
		return;
	}

	// if (DataTablePin->LinkedTo.Num() == 0)
	// {
	// 	const UDataTable* DataTable = Cast<UDataTable>(DataTablePin->DefaultObject);
	// 	if (!DataTable)
	// 	{
	// 		MessageLog.Error(*LOCTEXT("NoDataTable", "No DataTable in @@").ToString(), this);
	// 		return;
	// 	}

	// 	if (!RowNamePin->LinkedTo.Num())
	// 	{
	// 		const FName CurrentName = FName(*RowNamePin->GetDefaultAsString());
	// 		if (!DataTable->GetRowNames().Contains(CurrentName))
	// 		{
	// 			const FString Msg = FText::Format(
	// 				LOCTEXT("WrongRowNameFmt", "'{0}' row name is not stored in '{1}'. @@"),
	// 				FText::FromString(CurrentName.ToString()),
	// 				FText::FromString(GetFullNameSafe(DataTable))
	// 			).ToString();
	// 			MessageLog.Error(*Msg, this);
	// 			return;
	// 		}
	// 	}
	// }	
}

void UK2Node_AddInteraction::PreloadRequiredAssets()
{
	if (UEdGraphPin* DataTablePin = GetDataTablePin())
	{
		if (UDataTable* DataTable = Cast<UDataTable>(DataTablePin->DefaultObject))
		{
			// make sure to properly load the data-table object so that we can 
			// farm the "RowStruct" property from it (below, in GetDataTableRowStructType)
			PreloadObject(DataTable);
		}
	}
	return Super::PreloadRequiredAssets();
}

void UK2Node_AddInteraction::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetResultPin())
	{
		UEdGraphPin* TablePin = GetDataTablePin();
		// this connection would only change the output type if the table pin is undefined
		const bool bIsTypeAuthority = (TablePin->LinkedTo.Num() > 0 || TablePin->DefaultObject == nullptr);
		if (bIsTypeAuthority)
		{
			// RefreshOutputPinType();
		}		
	}
	else if (Pin == GetDataTablePin())
	{
		const bool bConnectionAdded = Pin->LinkedTo.Num() > 0;
		if (bConnectionAdded)
		{
			// if a connection was made, then we may need to rid ourselves of the row dropdown
			RefreshRowNameOptions();
			// if the output connection was previously, incompatible, it now becomes the authority on this node's output type
			// RefreshOutputPinType();
		}
	}
}

#undef LOCTEXT_NAMESPACE

