// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateQuest.h"
#include "../Interaction.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "../HUD/SFSHUD.h"
#include "../HUD/SFSHUDWidget.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"

FWidgetAnimationDynamicEvent AnimationFinished;

void UUpdateQuest::InitNode() {
	CurrentState = &UUpdateQuest::ShowWidget;
}

void UUpdateQuest::Wait() {
}

void UUpdateQuest::ShowWidget() {
	HUD = Cast<ASFSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (HUD) {
		UUserWidget* HUDWidget = HUD->HUDWidget;
		FName SlotName = ParentInteraction->QuestSlotName;
		if (SlotName == NAME_None) {
			SlotName = FName(HUD->GetWidgetSlotName());
			ParentInteraction->QuestSlotName = SlotName;
		}
		UIWidget = Cast<USFSHUDWidget>(HUDWidget->GetWidgetFromName(ParentInteraction->QuestSlotName));
		if (UIWidget) {
			QuestTitleTextBlock = Cast<UTextBlock>(UIWidget->GetWidgetFromName(TEXT("QuestTitle")));
			if (QuestTitleTextBlock) {
				QuestTitleTextBlock->SetText(FText::FromString(ParentInteraction->Name));
			}
			QuestStepTextBlock = Cast<UTextBlock>(UIWidget->GetWidgetFromName(TEXT("QuestStep")));
			if (QuestStepTextBlock) {
				ColorAndOpacity = QuestStepTextBlock->ColorAndOpacity;
			}
			AnimationFinished.BindUFunction(this, FName(TEXT("UpdateQuest")));
			AnimationToRun = Mute ? UIWidget->QuestTransitionMute : UIWidget->QuestTransition;
			UIWidget->PlayAnimation(AnimationToRun, 0, 1, EUMGSequencePlayMode::Forward, 1.3);
			UIWidget->BindToAnimationFinished(AnimationToRun, AnimationFinished);
			CurrentState = &UUpdateQuest::Wait;
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("UUpdateQuest: HUD Not Found."));
		 CurrentState = &UUpdateQuest::End;
	}
}

void UUpdateQuest::UpdateQuest() {
	QuestStepTextBlock->SetColorAndOpacity(ColorAndOpacity);
	QuestStepTextBlock->SetText(FText::FromString(QuestStepText));
	if (QuestStepText.IsEmpty()) {
		QuestTitleTextBlock->SetText(FText::FromString(TEXT("")));
	}
	UIWidget->UnbindAllFromAnimationFinished(AnimationToRun);
	UIWidget->PlayAnimation(AnimationToRun, 0, 1, EUMGSequencePlayMode::Reverse, 1.3);
	// if (InventoryConditionMet) {
		
	// 	CurrentState = &UUpdateQuest::End;	
	// } else {
	// 	CurrentState = &UUpdateQuest::Wait;	
	// }
	CurrentState = &UUpdateQuest::End;
}

// void UUpdateQuest::EvaluateCondition() {
// 	if (Inventory) {
// 		FString KeyToRemove;
// 		for (auto &&TransientStep : TransientQuestSteps) {
// 			bool TrasientItem = Inventory->CheckInventory(TransientStep.Key);
// 			if (TrasientItem) {
// 				UE_LOG(LogTemp, Warning, TEXT("TrasientItem in inventory %s"), *TransientStep.Key);
// 				QuestMessage = TransientStep.Value;
// 				KeyToRemove = TransientStep.Key;
// 				CurrentState =  &UUpdateQuest::ShowWidget;
// 			}
// 		}
// 		TransientQuestSteps.Remove(KeyToRemove);
		
// 		InventoryConditionMet = Inventory->CheckInventory(WhenInventoryItems);
//     	if (InventoryConditionMet) {
// 			UE_LOG(LogTemp, Warning, TEXT("QuestItem in inventory %s"), *WhenInventoryItems);
// 			QuestMessage = QuestStepText;
//         	CurrentState =  &UUpdateQuest::ShowWidget;
//     	}
// 	}
// }

// void UUpdateQuest::HideWidget() {
    // if (UIWidget) UIWidget->RemoveFromParent();
// }

// void UUpdateQuest::WaitShowWidgetTime() {
//     CurrentWaitingTime += GetWorld()->GetDeltaSeconds();
// 	if (CurrentWaitingTime > ShowWidgetTime) {
// 		CurrentWaitingTime = 0;
// 		CurrentState =  &UUpdateQuest::HideWidget;
// 	}
// }

bool UUpdateQuest::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UUpdateQuest::End;
}