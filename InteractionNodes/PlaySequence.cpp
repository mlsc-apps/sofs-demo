// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaySequence.h"
#include "LevelSequenceActor.h"
#include "../Interaction.h"
#include "../Interactable.h"
#include "MovieScene.h"

void UPlaySequence::InitNode() {

	// UMovieScene* MovieScene = Sequence->MovieScene;
	// if (MovieScene) {

	// MovieScene->DeleteMarkedFrames();
	// FMovieSceneMarkedFrame InMarkedFrame;
	// InMarkedFrame.FrameNumber = FFrameNumber(MovieScene->GetTickResolution().AsFrameNumber(1.0));
	// InMarkedFrame.Label = "There is this girl..";
	// // UE_LOG(LogTemp, Warning, TEXT("UPlaySequence: Frame Number %d"), InMarkedFrame.FrameNumber.Value);

	// FMovieSceneMarkedFrame InMarkedFrame2; // (FFrameNumber(165));
	// InMarkedFrame2.FrameNumber = FFrameNumber(MovieScene->GetTickResolution().AsFrameNumber(3.0));
	// InMarkedFrame2.Color = FLinearColor(FColor(255, 0, 0));
	// InMarkedFrame2.Label = "I can't help...";

	// 	// MovieScene->Modify();
	// 	MovieScene->AddMarkedFrame(InMarkedFrame);
	// 	MovieScene->AddMarkedFrame(InMarkedFrame2);
	// 	// FMovieSceneMarkedFrame InMarkedFrame = MovieScene->GetMarkedFrames()[0];
	// 	// UE_LOG(LogTemp, Warning, TEXT("UPlaySequence: Frame Number %d"), InMarkedFrame.FrameNumber.Value);
	// }
	CurrentState = &UPlaySequence::StartPlaying;
}

bool UPlaySequence::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UPlaySequence::End;
}

void UPlaySequence::StartPlaying() {
	ALevelSequenceActor* LevelSequencePlayer;
	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, PlaybackSettings, LevelSequencePlayer);
    if (Player) {
		Player->Initialize(Sequence, ParentInteraction->ParentInteractable->GetOwner()->GetLevel(), PlaybackSettings);
		Player->Play();
		if (WaitUntilEnded) {
				Player->OnStop.AddDynamic(this, &UPlaySequence::StopSequence);
		        CurrentState = &UPlaySequence::WaitingUntilSequenceEnded;
		} else {
			CurrentState = &UPlaySequence::End;
		}
	} else {
		 UE_LOG(LogTemp, Warning, TEXT("UPlaySequence: Level Sequence Player not initialized"));
	}
	// CurrentState = &UPlaySequence::End;
}

void UPlaySequence::WaitingUntilSequenceEnded() {
}

void UPlaySequence::StopSequence() {
	UE_LOG(LogTemp, Warning, TEXT("UPlaySequence: StopSequence Entered"));
	CurrentState = &UPlaySequence::End;
}