// Copyright Epic Games, Inc. All Rights Reserved.

#include "SGraphPinEFinalizeInteraction.h"
#include "../Interactable.h"

void SGraphPinEFinalizeInteraction::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	// DataTable = InDataTable;
	RefreshNameList();
	SGraphPinNameList::Construct(SGraphPinNameList::FArguments(), InGraphPinObj, NameList);
}

SGraphPinEFinalizeInteraction::SGraphPinEFinalizeInteraction() {}

SGraphPinEFinalizeInteraction::~SGraphPinEFinalizeInteraction() {}

// void SGraphPinEFinalizeInteraction::PreChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info)
// {
// }

// void SGraphPinEFinalizeInteraction::PostChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info)
// {
	//FSoftObjectPath::InvalidateTag(); // Should be removed after UE-5615 is fixed
	// if (Changed && (Changed == DataTable.Get()) && (FDataTableEditorUtils::EDataTableChangeInfo::RowList == Info))
	// {
	// 	RefreshNameList();
	// }
// }

void SGraphPinEFinalizeInteraction::RefreshNameList()
{
	NameList.Empty();

	for (EFinalizeWith Val : TEnumRange<EFinalizeWith>()) {
		if (Val == EFinalizeWith::Reset) {
			TSharedPtr<FName> Item = MakeShareable(new FName(TEXT("Reset")));
			NameList.Add(Item);
		}
		if (Val == EFinalizeWith::Destroy) {
			TSharedPtr<FName> Item = MakeShareable(new FName(TEXT("Destroy")));
			NameList.Add(Item);
		}
	 }

	//  NameList.Add(FinalizeWith);

	// if (DataTable.IsValid())
	// {
	// 	auto Names = DataTable->GetRowNames();
	// 	for (auto Name : Names)
	// 	{
	// 		TSharedPtr<FName> RowNameItem = MakeShareable(new FName(Name));
	// 		NameList.Add(RowNameItem);
	// 	}
	// }
}
