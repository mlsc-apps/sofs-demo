// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "ApplyImpulse.generated.h"

class AActor;

/**
 * 
 */
UCLASS()
class MUKA_API UApplyImpulse : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UApplyImpulse::* Action)(void);
	Action CurrentState;
	AActor* Target;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ActorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Force;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ForceLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Sound;
	
	void InitNode() override;
	bool Exec() override;

	void StartApplyingImpulse();

};
