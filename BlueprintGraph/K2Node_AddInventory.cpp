// Fill out your copyright notice in the Description page of Project Settings.

#include "K2Node_AddInventory.h"
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
// #include "../Interactable.h"
// #include "../Interaction.h"
// #include "../InteractionNodes/AddInventory.h"
#include "../Inventory.h"
#include "BlueprintActionDatabaseRegistrar.h"

#define LOCTEXT_NAMESPACE "UK2Node_AddInventory"

UK2Node_AddInventory::UK2Node_AddInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Adds Add Inventory node to Interaction Object");
}

void UK2Node_AddInventory::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	UEdGraphPin* ExecutePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* ThenPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

    // Add Input Pins
	UEdGraphPin* PreviousNodePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteractionNode::StaticClass(), AddInventoryPinNameHelper::PreviousNodePin);
	UEdGraphPin* InteractionPin  = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UInteraction::StaticClass(), AddInventoryPinNameHelper::InteractionPin);
	UEdGraphPin* DataTablePin    = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UDataTable::StaticClass(), AddInventoryPinNameHelper::DataTablePin);
	UEdGraphPin* RowNamePin      = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FMUKAInventoryItemSpec::StaticStruct(), AddInventoryPinNameHelper::RowNamePin);
	// UEdGraphPin* InventoryItemNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, AddInventoryPinNameHelper::InventoryItemNamePin);
	// UEdGraphPin* DescriptionPin  = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, AddInventoryPinNameHelper::DescriptionPin);
	// UEdGraphPin* ActorTagPin     = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, AddInventoryPinNameHelper::ActorTagPin);
	UEdGraphPin* WaitBeforePin   = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Float, AddInventoryPinNameHelper::WaitBeforePin);
	WaitBeforePin->DefaultValue  = "0.0";
    UEdGraphPin* ActivePin       = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Boolean, AddInventoryPinNameHelper::ActivePin); 
	ActivePin->DefaultValue = "true";
    
	// Output Pins
	UEdGraphPin* ResultPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Object, UInteractionNode::StaticClass(), UEdGraphSchema_K2::PN_ReturnValue);
	ResultPin->PinFriendlyName = LOCTEXT("This Node", "This Node");

	Super::AllocateDefaultPins();
}

void UK2Node_AddInventory::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
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

// void UK2Node_AddInventory::RefreshOutputPinType()
// {
// 	UScriptStruct* OutputType = GetDataTableRowStructType();
// 	SetReturnTypeForStruct(OutputType);
// }

void UK2Node_AddInventory::RefreshRowNameOptions()
{
	// When the DataTable pin gets a new value assigned, we need to update the Slate UI so that SGraphNodeCallParameterCollectionFunction will update the ParameterName drop down
	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}

UEdGraphPin* UK2Node_AddInventory::GetResultPin() const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_AddInventory::GetThenPin()const
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* Pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_AddInventory::GetDataTablePin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;
    
	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* TestPin : *PinsToSearch)
	{
		if (TestPin && TestPin->PinName == AddInventoryPinNameHelper::DataTablePin)
		{
			Pin = TestPin;
			break;
		}
	}
	check(Pin == nullptr || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_AddInventory::GetRowNamePin() const
{
	UEdGraphPin* Pin = FindPinChecked(AddInventoryPinNameHelper::RowNamePin);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UScriptStruct* UK2Node_AddInventory::GetReturnTypeForStruct()
{
	UScriptStruct* ReturnStructType = (UScriptStruct*)(GetResultPin()->PinType.PinSubCategoryObject.Get());

	return ReturnStructType;
}

UScriptStruct* UK2Node_AddInventory::GetDataTableRowStructType() const
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

void UK2Node_AddInventory::OnDataTableRowListChanged(const UDataTable* DataTable)
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

void UK2Node_AddInventory::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) 
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

void UK2Node_AddInventory::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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

// FText UK2Node_AddInventory::GetMenuCategory() const
// {
// 	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
// }

bool UK2Node_AddInventory::IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const {
	if (MyPin == GetResultPin() && MyPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard) {
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

void UK2Node_AddInventory::RefreshFromDataTableRow(FName RowName) {
    FString ContextString;
	UDataTable* Table = Cast<UDataTable>(GetDataTablePin()->DefaultObject);
	if (Table) {
		FMUKAInventoryItemSpec* RowPtr = Table->FindRow<FMUKAInventoryItemSpec>(RowName, ContextString);
		if (RowPtr) {
			FMUKAInventoryItemSpec tempSpec;
			tempSpec.Name = RowName.ToString();
			tempSpec.ActorTag = RowPtr->ActorTag;
			tempSpec.Description = RowPtr->Description;
		// (FindPinChecked(AddInventoryPinNameHelper::RowNamePin))->DefaultObject = tempSpec;
		// FindPinChecked(AddInventoryPinNameHelper::DescriptionPin)->DefaultValue = RowPtr->Description;
		// FindPinChecked(AddInventoryPinNameHelper::InventoryItemNamePin)->DefaultValue = RowName.ToString();
		}
	}

}

void UK2Node_AddInventory::PinDefaultValueChanged(UEdGraphPin* ChangedPin) {
	if (ChangedPin && ChangedPin->PinName == AddInventoryPinNameHelper::DataTablePin) {
		// RefreshOutputPinType();

		UEdGraphPin* RowNamePin = FindPinChecked(AddInventoryPinNameHelper::RowNamePin);
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

	if (ChangedPin && ChangedPin->PinName == AddInventoryPinNameHelper::RowNamePin) {
		RefreshFromDataTableRow(FName(ChangedPin->DefaultValue));
	}

}

FText UK2Node_AddInventory::GetTooltipText() const
{
	return NodeTooltip;
}

FText UK2Node_AddInventory::GetNodeTitle(ENodeTitleType::Type TitleType) const {
    return LOCTEXT("MUKA_NodeTitle", "Add To Inventory");
}

// FText UK2Node_AddInventory::GetTooltipText() const {
//     return LOCTEXT("SOU_Interaction_Tooltip", "This is our Tutorial K2 Node Tooltip information!");
// }

FText UK2Node_AddInventory::GetMenuCategory() const {
    return LOCTEXT("MUKA_MenuCategory", "MUKA");
}

void UK2Node_AddInventory::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) {
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
	const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UMUKABlueprintFunctionLibrary, AddToInventory);
	UK2Node_CallFunction* Outer = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	Outer->FunctionReference.SetExternalMember(FunctionName, UMUKABlueprintFunctionLibrary::StaticClass());
	Outer->AllocateDefaultPins();
    CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(Outer->GetExecPin()));

	UEdGraphPin* InnerReturnValuePin = FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    UEdGraphPin* OuterReturnValuePin = Outer->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);
    OuterReturnValuePin->PinType = InnerReturnValuePin->PinType;
	OuterReturnValuePin->PinType.PinSubCategoryObject = InnerReturnValuePin->PinType.PinSubCategoryObject;
	CompilerContext.MovePinLinksToIntermediate(*InnerReturnValuePin, *OuterReturnValuePin);

    CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(AddInventoryPinNameHelper::InteractionPin), *Outer->FindPinChecked(TEXT("Interaction")));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(AddInventoryPinNameHelper::PreviousNodePin), *Outer->FindPinChecked(TEXT("PreviousNode")));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(AddInventoryPinNameHelper::RowNamePin), *Outer->FindPinChecked(TEXT("InventoryItem")));
	CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(AddInventoryPinNameHelper::WaitBeforePin), *Outer->FindPinChecked(TEXT("WaitBefore")));
    CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(AddInventoryPinNameHelper::ActivePin), *Outer->FindPinChecked(TEXT("Active")));
	

    UEdGraphPin* OuterThenPin = Outer->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *OuterThenPin);

	BreakAllNodeLinks();
}

FSlateIcon UK2Node_AddInventory::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_AddInventory::PostReconstructNode()
{
	Super::PostReconstructNode();

	// RefreshOutputPinType();
}

void UK2Node_AddInventory::EarlyValidation(class FCompilerResultsLog& MessageLog) const
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

void UK2Node_AddInventory::PreloadRequiredAssets()
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

void UK2Node_AddInventory::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
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