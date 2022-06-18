// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "ItemGateway.generated.h"

class AInventory;

/**
 * 
 */
UCLASS()
class MUKA_API UItemGateway : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UItemGateway::* Action)(void);
	Action CurrentState;
	Action WaitCallback;
	AInventory* Inventory;

protected:
	float CurrentWaitingTime = 0;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float Interval;
	FString WhenInventoryItems;

	void InitNode() override;
	bool Exec() override;
	void Wait();
	UFUNCTION()
	void EvaluateCondition();
	
};
