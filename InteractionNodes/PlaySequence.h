// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequencePlayer.h"
// #include "LevelSequence.h"
#include "PlaySequence.generated.h"

class ULevelSequence;

/**
 * 
 */
UCLASS()
class MUKA_API UPlaySequence : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UPlaySequence::* Action)(void);
	Action CurrentState;
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevelSequence* Sequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WaitUntilEnded;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// FLevelSequenceCameraSettings InCameraSettings;
	
	void InitNode() override;
	bool Exec() override;

	void StartPlaying();
	void WaitingUntilSequenceEnded();
	UFUNCTION()
	void StopSequence();
	
};
