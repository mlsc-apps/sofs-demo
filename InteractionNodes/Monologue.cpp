// Fill out your copyright notice in the Description page of Project Settings.


#include "Monologue.h"
// #include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Interaction.h"
#include "../Interactable.h"

void UMonologue::InitNode() {
	UE_LOG(LogTemp, Warning, TEXT("UMonologue: Init"));
	CurrentState = &UMonologue::StartTalking;
	LineIndex = 0;
}

bool UMonologue::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UMonologue::End;
}

void UMonologue::StartTalking() {
	    AActor *Speaker = nullptr;
		Speaker = GetActorByTag(SpeakerTag);
		if (Speaker) {
			TextCloud = GetSpeakersTextCloud(Speaker);
			SayText.ParseIntoArray(Lines, TEXT("\n"), false);
			CurrentState = &UMonologue::SayNextLine;
		} else {
			UE_LOG(LogTemp, Error, TEXT("UMonologue: No Speaker %s for Monologue found!"), *SpeakerTag.ToString());
			CurrentState = &UMonologue::End;
		}
}

URichTextBlock* UMonologue::GetSpeakersTextCloud(AActor* Speaker) {
		UWidgetComponent *widgetcomponent = Speaker->FindComponentByClass<UWidgetComponent>();
		if (widgetcomponent) {
			FVector WorldPosition = Speaker->GetActorLocation();
			WorldPosition.X = UKismetMathLibrary::Round(widgetcomponent->GetComponentLocation().X);
			WorldPosition.Y = UKismetMathLibrary::Round(widgetcomponent->GetComponentLocation().Y);
			WorldPosition.Z = UKismetMathLibrary::Round(widgetcomponent->GetComponentLocation().Z);
			widgetcomponent->SetWorldLocation(WorldPosition);
			TextCloud = Cast<URichTextBlock>(widgetcomponent->GetUserWidgetObject()->GetWidgetFromName("TextCloud"));
			UInteractable *Interactable = Speaker->FindComponentByClass<UInteractable>();
			if (TextCloud && Interactable) {
				TextCloud->SetDefaultColorAndOpacity(Interactable->TextCloudColor);
			}
		}
	return TextCloud;
}

void UMonologue::SayNextLine() {
	if (LineIndex < Lines.Num()) {
		CurrentState = &UMonologue::SayLine;
	} else {
		TextCloud->SetText(FText::FromString(TEXT("")));
		CurrentState = &UMonologue::End;
	}
}

void UMonologue::SayLine() {
	CurrentLine = Lines[LineIndex++];
	URichTextBlock* TextCloudToSay = GetTextCloud();
	if (TextCloudToSay) {
		TextCloudToSay->SetText(FText::FromString(GetProcessedLine()));
	} else {
		UE_LOG(LogTemp, Error, TEXT("UMonologue: No TextCloud Found! for line %s"), *CurrentLine);
	}
	WaitCallback = &UMonologue::SayNextLine;
	CurrentState = &UMonologue::Wait;
}

FString UMonologue::GetProcessedLine() {
	return CurrentLine;
}

URichTextBlock* UMonologue::GetTextCloud() {
	return TextCloud;
}

// void UMonologue::CleanTextClouds() {
     
// }

void UMonologue::Wait() {
	CurrentWaitingTime += GetWorld()->GetDeltaSeconds();
	if (CurrentWaitingTime > Delay) {
		// UE_LOG(LogTemp, Warning, TEXT("Elapsed %f"), CurrentWaitingTime);
		CurrentWaitingTime = 0;
		CurrentState = WaitCallback;
	}
}
