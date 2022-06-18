// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "CompositeItemGateway.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UCompositeItemGateway : public UInteractionNode
{
	GENERATED_BODY()
	
private:
	typedef void (UCompositeItemGateway::* Action)(void);
	Action CurrentState;
	AInventory* Inventory;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UInteractionNode*> NodesMap;
	
	void InitNode() override;
	bool Exec() override;
	void Wait();
	
	UFUNCTION()
	void OnInventoryUpdate();

};
