// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Animation/WidgetAnimation.h"
#include "QuestItemGateway.generated.h"

class ASFSHUD;
class UUMGSequencePlayer;
class UTextBlock;
class USFSHUDWidget;
class UUpdateQuest;

/**
 * 
 */
UCLASS()
class MUKA_API UQuestItemGateway : public UInteractionNode
{
	GENERATED_BODY()

private:
	typedef void (UQuestItemGateway::* Action)(void);
	Action CurrentState;
	AInventory* Inventory;
	float CurrentWaitingTime = 0;
	UUMGSequencePlayer* AnimPlayer;
	FSlateColor ColorAndOpacity;
	UTextBlock* QuestStepTextBlock;
	UTextBlock* QuestTitleTextBlock;
	USFSHUDWidget* UIWidget;
	FString QuestMessage;
	bool InventoryConditionMet = false;
	// void RemovePreviousNode(UInteractionNode* ThisNode);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WhenInventoryItems;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// USoundBase* Sound;
	UPROPERTY(EditAnywhere, Category = "MUKA")
	TSubclassOf<class UUserWidget> QuestWidget;
	UPROPERTY(EditAnywhere, Category = "MUKA")
	ASFSHUD* HUD;
	UPROPERTY(EditAnywhere, Category = "MUKA")
	float ShowWidgetTime;
	UPROPERTY(EditAnywhere, Category = "MUKA")
	FString QuestStepText;
	UPROPERTY(EditAnywhere, Category = "MUKA")
	UUpdateQuest* UpdateQuestNode;

	void InitNode() override;
	bool Exec() override;
	void Wait();
	// void ShowWidget();
	// UFUNCTION()
	// void UpdateQuest();
	UFUNCTION()
	void EvaluateCondition();
	
};
