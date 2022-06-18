// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Branch.generated.h"

class UChoice;

/**
 * 
 */
UCLASS()
class MUKA_API UBranch : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UBranch::* Action)(void);
	Action CurrentState;
	
public:
    UChoice* ConversationChoice;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BranchName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WhenInventoryItems;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Timeout;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// int32 BranchIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RunOnce;
	
	void InitNode() override;
	bool Exec() override;
	
	UFUNCTION()
	void UpdateConversationChoiceNextNode();
};
