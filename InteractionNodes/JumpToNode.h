// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "JumpToNode.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UJumpToNode : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UJumpToNode::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInteractionNode* JumpNode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString JumpNodeName;

	
	void InitNode() override;
	bool Exec() override;

	void JumpToNode();

};
