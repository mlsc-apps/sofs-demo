// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Wait.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UWait : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UWait::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY()
	float WaitTime;
	UPROPERTY()
	float CurrentWaitingTime = 0;
	
	void InitNode() override;
	bool Exec() override;

	void Wait();
	
};
