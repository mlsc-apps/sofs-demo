// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "DestroyInteraction.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UDestroyInteraction : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UDestroyInteraction::* Action)(void);
	Action CurrentState;
	
public:	
	void InitNode() override;
	bool Exec() override;

	void StartDestroying();
	
};
