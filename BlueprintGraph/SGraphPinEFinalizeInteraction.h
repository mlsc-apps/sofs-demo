// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SGraphPinNameList.h"
#include "Engine/DataTable.h"
#include "DataTableEditorUtils.h"

class MUKA_API SGraphPinEFinalizeInteraction : public SGraphPinNameList
{
public:
	SLATE_BEGIN_ARGS(SGraphPinEFinalizeInteraction) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

	SGraphPinEFinalizeInteraction();
	virtual ~SGraphPinEFinalizeInteraction();

	// FDataTableEditorUtils::INotifyOnDataTableChanged
	// virtual void PreChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info) override;
	// virtual void PostChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo Info) override;

protected:

	void RefreshNameList();

	// TSoftObjectPtr<enum class EFinalizeWith> DataTable;
};
