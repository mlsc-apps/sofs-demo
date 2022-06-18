// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "PlayAnimation.generated.h"

class UAnimSequence;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class MUKA_API UPlayAnimation : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UPlayAnimation::* Action)(void);
	Action CurrentState;
	Action WaitCallback;
    float CurrentWaitingTime = 0;
	float MontageLengthInSeconds = 0;
	AActor* AnimationActor;
	USkeletalMeshComponent* Mesh;
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence *Animation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage *Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Loop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ActorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WaitUntilEnded;
	
	void InitNode() override;
	bool Exec() override;

	void StartPlayingAnimation();
	void StartPlayingMontage();
	void WaitingUntilAnimationEnded();
	void WaitingUntilMontageEnded();
	
	
};
