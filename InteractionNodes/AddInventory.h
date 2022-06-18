// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "../Inventory.h"
#include "AddInventory.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UAddInventory : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UAddInventory::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMUKAInventoryItemSpec ItemSpecification;
	
	void InitNode() override;
	bool Exec() override;

	void StartAddingToInventory();
};
