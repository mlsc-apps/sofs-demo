// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Fork.generated.h"

// class UInteractionNode;

// USTRUCT(BlueprintType)
// struct FCondition {

// 	GENERATED_BODY()

//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fork Condition")
// 	FString Name;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fork Condition")
// 	TArray<FString> InventoryWhiteList;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fork Condition")
// 	TArray<FString> InventoryBlackList;
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fork Condition")
// 	FString BranchName;

// };

/**
 * 
 */
UCLASS()
class MUKA_API UFork : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UFork::* Action)(void);
	Action CurrentState;
	AInventory* Inventory;
	
public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// FString WhenInventoryItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UInteractionNode*> NodesMap;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fork Condition")
	// FString DefaultBranch;
	
	void InitNode() override;
	bool Exec() override;

	void StartForking();
};
