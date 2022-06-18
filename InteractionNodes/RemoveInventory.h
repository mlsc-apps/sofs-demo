// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "RemoveInventory.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API URemoveInventory : public UInteractionNode
{
	GENERATED_BODY()
	
private:
	typedef void (URemoveInventory::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	void InitNode() override;
	bool Exec() override;

	void StartingRemovingFromInventory();
};
