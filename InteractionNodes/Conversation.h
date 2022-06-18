// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Conversation.generated.h"

class ASFSHUD;
class UTextBlock;
class USoundBase;

/**
 * 
 */
UCLASS()
class MUKA_API UConversation : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UConversation::* Action)(void);
	Action CurrentState;
	Action WaitCallback;
	TArray<AActor*> Speakers;
	TArray<AActor*> Cameras;
	AActor* CurrentCamera;
	AActor* PreviousCamera;
	FName CurrentSpeaker;
	
	ASFSHUD* HUD;
	UUserWidget* SubtitlesWidget;
	
	int LineIndex = 0;
	float CurrentWaitingTime = 0;
	float MaxWaitTime = 0;

	TArray<FString> Lines;
	FString CurrentLine;
	UTextBlock* Subtitles;

	void MoveToNextLine();
	void SayLine();
	void SetCurrentLine();
	void ChangeCameraForLine();
	void DelayBeforeLine();
	AActor* GetCamera(FName CameraForSpeaker, FName CameraName);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpeakerToShow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Speaker0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Speaker0Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Speaker1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Speaker1Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Script;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayBetweenLines;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayBetweenSpeakers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ChangeCameras;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShowSpeakers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> Speech;
	
	void InitNode() override;
	bool Exec() override;
	
	void Wait();
	// void StartTalking();
};
