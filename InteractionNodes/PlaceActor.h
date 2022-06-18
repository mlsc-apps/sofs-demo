// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "PlaceActor.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UPlaceActor : public UInteractionNode
{
	GENERATED_BODY()

	private:
	typedef void (UPlaceActor::* Action)(void);
	Action CurrentState;
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	
	void InitNode() override;
	bool Exec() override;

	void StartPlacingActor();
	
};
