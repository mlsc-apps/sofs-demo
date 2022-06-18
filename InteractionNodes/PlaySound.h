// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "PlaySound.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API UPlaySound : public UInteractionNode
{
	GENERATED_BODY()
	
	private:
	typedef void (UPlaySound::* Action)(void);
	Action CurrentState;
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Sound;
	
	void InitNode() override;
	bool Exec() override;

	void StartPlayingSound();
};
