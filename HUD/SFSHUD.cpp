// Fill out your copyright notice in the Description page of Project Settings.


#include "SFSHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

bool ASFSHUD::CheckSlot(FString SlotName) {
    UUserWidget* UIWidget = Cast<UUserWidget>(HUDWidget->GetWidgetFromName(FName(SlotName)));
	if (UIWidget) {
		UTextBlock* QuestTitleTextBlock = Cast<UTextBlock>(UIWidget->GetWidgetFromName(TEXT("QuestTitle")));
        return QuestTitleTextBlock->GetText().IsEmpty();
    }
    return false;
}

FString ASFSHUD::GetWidgetSlotName() {
    if (CheckSlot("WBP_Quest1")) return "WBP_Quest1";
    if (CheckSlot("WBP_Quest2")) return "WBP_Quest2";
    if (CheckSlot("WBP_Quest3")) return "WBP_Quest3";
    return "WBP_Quest1";
}

UUserWidget* ASFSHUD::GetConversationChoiceWidget() {
    return Cast<UUserWidget>(HUDWidget->GetWidgetFromName(FName("ConversationChoiceWidget")));
}

UUserWidget* ASFSHUD::GetSubtitlesWidget() {
    return Cast<UUserWidget>(HUDWidget->GetWidgetFromName(FName("SubtitlesWidget")));
}