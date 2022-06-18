// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Use.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UUse : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UUse::* Action)(void);
	Action CurrentState;
	// FString ItemChosen;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UInteractionNode*> NodesMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInventory* Inventory;
	
	void InitNode() override;
	bool Exec() override;
	
	UFUNCTION()
	void WaitForPlayerChoice();
	UFUNCTION()
	void OpenInventoryWidget();
	UFUNCTION()
	void SetItemChosen(FString ItemTag);

};
