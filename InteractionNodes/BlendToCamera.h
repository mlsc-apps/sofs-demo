// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "BlendToCamera.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UBlendToCamera : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UBlendToCamera::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime;
	
	void InitNode() override;
	bool Exec() override;

	void StartBlendingToCamera();
	
};
