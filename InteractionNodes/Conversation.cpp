// Fill out your copyright notice in the Description page of Project Settings.


#include "Conversation.h"
#include "../HUD/SFSHUD.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UConversation::InitNode() {
	LineIndex = 0;

	// Speakers.Add(GetActorByTag(Speaker0));
	// Speakers.Add(GetActorByTag(Speaker1));

	Cameras.Add(GetCamera(Speaker0, Speaker0Camera));
	Cameras.Add(GetCamera(Speaker1, Speaker1Camera));
    
	Script.ParseIntoArray(Lines, TEXT("\n"), false);
	HUD = Cast<ASFSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HUD) {
			SubtitlesWidget = HUD->GetSubtitlesWidget();
        	if (SubtitlesWidget) {
				Subtitles = Cast<UTextBlock>(SubtitlesWidget->GetWidgetFromName(FName("Subtitles")));
				if (Subtitles) Subtitles->SetText(FText::FromString(TEXT("")));
				CurrentState = &UConversation::ChangeCameraForLine;
			} else {
				CurrentState = &UConversation::End;
			}
	}
	PreviousCamera = nullptr;
}
	

// void UConversation::StartTalking() {
    // for (auto &&ConversationSpeakerTag : SpeakerTags) {
    //     AActor* Speaker = GetActorByTag(ConversationSpeakerTag);
    //     if (Speaker) {
    //         SpeakerTextClouds.Add(GetSpeakersTextCloud(Speaker));
	// 	} else {
	// 		UE_LOG(LogTemp, Warning, TEXT("UConversation: Speaker with Tag %s not found!"), *ConversationSpeakerTag.ToString());
	// 	}
    // }

	
// }

void UConversation::ChangeCameraForLine() {
	if (LineIndex < Lines.Num()) {
		// CurrentState = &UConversation::SetCurrentLine;
		CurrentLine = Lines[LineIndex];
		UE_LOG(LogTemp, Warning, TEXT("UConversation: Current Line %s"), *CurrentLine);
		FString SpeakerIndex;
		CurrentLine.Split(":", &SpeakerIndex, &CurrentLine);
    	int index = FCString::Atoi(*SpeakerIndex);
		CurrentSpeaker = (SpeakerToShow != NAME_None) ? SpeakerToShow : index == 0 ? Speaker0 : Speaker1;
		if (Subtitles) {
			if (ChangeCameras) {
				CurrentCamera = Cameras[index];
				if (CurrentCamera && CurrentCamera != PreviousCamera) {
						PreviousCamera = CurrentCamera;
						UE_LOG(LogTemp, Warning, TEXT("UConversation: Move to Camera %s"), *CurrentCamera->GetName());
						GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CurrentCamera);
						MaxWaitTime = DelayBetweenSpeakers;
						WaitCallback = &UConversation::SayLine;
						CurrentState = &UConversation::Wait;
					} else {
						CurrentState =  &UConversation::SayLine;
					}
			} else {
				CurrentState =  &UConversation::SayLine;
			}
		}
	} else {
		SubtitlesWidget->SetVisibility(ESlateVisibility::Collapsed);
		CurrentState = &UConversation::End;
	}
}

void UConversation::SayLine() {
	if (ShowSpeakers) {
		FString SpeakerActorSubtitle = CurrentSpeaker.ToString().Append(": ").Replace(TEXT("Vincent"), TEXT("Hero"));
		CurrentLine = SpeakerActorSubtitle.Append(CurrentLine.TrimEnd());
	}
	Subtitles->SetText(FText::FromString(CurrentLine.TrimEnd()));
	SubtitlesWidget->SetVisibility(ESlateVisibility::Visible);
	
	if (LineIndex < Speech.Num()) {
		USoundBase* SpeechToSay = Speech[LineIndex];
		if (SpeechToSay) UGameplayStatics::PlaySound2D(GetWorld(), SpeechToSay);
	}

	MaxWaitTime = DelayBetweenLines;
	WaitCallback = &UConversation::MoveToNextLine;
	CurrentState = &UConversation::Wait;
}

void UConversation::DelayBeforeLine() {
	MaxWaitTime = DelayBetweenSpeakers;
	WaitCallback = &UConversation::SayLine;
	CurrentState = &UConversation::Wait;
	if (Subtitles) Subtitles->SetText(FText::FromString(TEXT("")));
}

void UConversation::MoveToNextLine() {
	LineIndex++;
	if (Subtitles) Subtitles->SetText(FText::FromString(TEXT("")));
	MaxWaitTime = DelayBetweenSpeakers;
	WaitCallback = &UConversation::ChangeCameraForLine;
	CurrentState = &UConversation::Wait;
	// CurrentState = &UConversation::ChangeCameraForLine;
}


AActor* UConversation::GetCamera(FName CameraForSpeaker, FName CameraName) {
    AActor* CamSpeaker = GetActorByTag(CameraForSpeaker);
     if (CamSpeaker) {
        TArray <AActor*> ChildActors;
        CamSpeaker->GetAttachedActors(ChildActors);
        UE_LOG(LogTemp, Warning, TEXT("UChoice: Child Actors Array Length %d"), ChildActors.Num());
        for (auto &&ChildActor : ChildActors) {
            // UE_LOG(LogTemp, Warning, TEXT("UChoice: Tag 0 %s"), *ChildActor->Tags[0].ToString());
            if (ChildActor->ActorHasTag(CameraName)) {
                UE_LOG(LogTemp, Warning, TEXT("UChoice: Found Camera %s"), *ChildActor->GetName());
                // GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(ChildActor);
                return ChildActor;
            }
        }
     }
    //  CurrentState = &UChoice::StartChoices;
	return nullptr;
}

// void UConversation::SetCurrentLine() {
// 	CurrentLine = Lines[LineIndex];
// 	if (CurrentSpeakingTextCloud) {
// 		CurrentSpeakingTextCloud->SetText(FText::FromString(TEXT("")));
// 	}
// 	URichTextBlock* NewCloud = GetTextCloud();
// 	if (NewCloud == CurrentSpeakingTextCloud) {
// 		CurrentState = &UConversation::SayLine;
// 	} else {
// 		CurrentSpeakingTextCloud = NewCloud;
// 		WaitCallback = &UConversation::SayLine;
// 		MaxWaitTime = LineIndex == 0 ? 0 : DelayBetweenSpeakers;
// 		CurrentState = &UConversation::Wait;
// 	}
// 	LineIndex++;
// }

// void UConversation::SayLine() {
// 	if (CurrentSpeakingTextCloud) {
// 		CurrentSpeakingTextCloud->SetText(FText::FromString(GetProcessedLine()));
// 	} else {
// 		UE_LOG(LogTemp, Warning, TEXT("UConversation: No TextCloud Found! for line %s"), *CurrentLine);
// 	}
// 	WaitCallback = &UConversation::SayNextLine;
// 	MaxWaitTime = Delay;
// 	CurrentState = &UConversation::Wait;
// }

// FString UConversation::GetProcessedLine() {
//     FString SpeakerIndex;
// 	CurrentLine.Split(":", &SpeakerIndex, &CurrentLine);
//     return CurrentLine;
// }

// URichTextBlock* UConversation::GetTextCloud() {
//     FString SpeakerIndex;
//     FString LineToSay;
//     CurrentLine.Split(":", &SpeakerIndex, &LineToSay);
//     int index = FCString::Atoi(*SpeakerIndex);
// 	if (index < SpeakerTextClouds.Num()) {
// 		return SpeakerTextClouds[index];
// 	} else {
// 		UE_LOG(LogTemp, Warning, TEXT("UConversation: Speaker with Index %i not in Speaker array!"), index);
// 	}
//     return nullptr;
// }

// void UConversation::CleanTextClouds() {
//     for (auto &&ConversationTextCloud : SpeakerTextClouds) {
//         ConversationTextCloud->SetText(FText::FromString(TEXT("")));
//     }
// }

bool UConversation::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UConversation::End;
}

void UConversation::Wait() {
	CurrentWaitingTime += GetWorld()->GetDeltaSeconds();
	if (CurrentWaitingTime > MaxWaitTime) {
		CurrentWaitingTime = 0;
		CurrentState = WaitCallback;
	}
}