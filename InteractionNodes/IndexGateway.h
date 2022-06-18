// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "IndexGateway.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UIndexGateway : public UInteractionNode
{
	GENERATED_BODY()
	
private:
	typedef void (UIndexGateway::* Action)(void);
	Action CurrentState;
	int CurrentCount = 0;
	UInteractionNode* OriginalNextNode;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaximumCount;
	
	void InitNode() override;
	bool Exec() override;

	void EvaluateCondition();
};
