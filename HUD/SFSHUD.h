// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SFSHUD.generated.h"

 /* 
 */
UCLASS()
class MUKA_API ASFSHUD : public AHUD
{
	GENERATED_BODY()

private:
    bool CheckSlot(FString Slotname);

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* HUDWidget;

	FString GetWidgetSlotName();
	UUserWidget* GetConversationChoiceWidget();
	UUserWidget* GetSubtitlesWidget();

	// void UpdateQuestStepDescription(FString QuestStepDescription);
	
};
