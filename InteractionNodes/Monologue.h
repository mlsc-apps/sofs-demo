// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "GameFramework/Character.h"
#include "Components/RichTextBlock.h"
#include "Monologue.generated.h"

USTRUCT(BlueprintType)
struct FMUKADialogueList : public FTableRowBase {

	GENERATED_USTRUCT_BODY()

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Maeso Ultimate Klick Adventure System (MUKA)")
	// FString RowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString Line;

};

/**
 * 
 */
UCLASS()
class MUKA_API UMonologue : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UMonologue::* Action)(void);
	Action CurrentState;
	Action WaitCallback;
	URichTextBlock *TextCloud;

protected:
	float CurrentWaitingTime = 0;
	TArray<FString> Lines;
	int LineIndex = 0;
	FString CurrentLine;
	virtual URichTextBlock* GetTextCloud();
	// virtual void CleanTextClouds();
	virtual URichTextBlock* GetSpeakersTextCloud(AActor* Speaker);
	virtual FString GetProcessedLine();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SayText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpeakerTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay;

	void InitNode() override;
	bool Exec() override;
	void WaitBeforeSpeaking();
	void StartTalking();
	virtual void SayNextLine();
	virtual void SayLine();
	virtual void Wait();
	
};
