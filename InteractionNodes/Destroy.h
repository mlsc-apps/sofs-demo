// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Destroy.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UDestroy : public UInteractionNode
{
	GENERATED_BODY()

	private:
	typedef void (UDestroy::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Tag;
	
	void InitNode() override;
	bool Exec() override;

	void StartDestroying();
	
};
