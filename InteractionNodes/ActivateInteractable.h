// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "ActivateInteractable.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UActivateInteractable : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UActivateInteractable::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY()
	FName ActorTag;
	UPROPERTY()
	bool Activate;
	
	void InitNode() override;
	bool Exec() override;

	void StartChangingState();
};
