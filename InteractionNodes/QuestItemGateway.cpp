// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestItemGateway.h"
#include "../Inventory.h"
#include "../Interaction.h"
// #include "Blueprint/UserWidget.h"
// #include "Kismet/GameplayStatics.h"
// #include "Components/TextBlock.h"
// #include "../HUD/SFSHUD.h"
// #include "../HUD/SFSHUDWidget.h"
// #include "Animation/UMGSequencePlayer.h"
#include "UpdateQuest.h"

// FWidgetAnimationDynamicEvent AnimationFinished;

void UQuestItemGateway::InitNode() {
    // WaitCallback = &UQuestItemGateway::EvaluateCondition;
	if (!Inventory) {
		Inventory = GetInventory();
	}
	if (Inventory) {
		Inventory->ItemChangedDelegate.AddDynamic(this, &UQuestItemGateway::EvaluateCondition);
		UE_LOG(LogTemp, Warning, TEXT("UQuestItemGateway Registered"));
	}
	CurrentState = &UQuestItemGateway::Wait;
}

void UQuestItemGateway::Wait() {
}

// void UQuestItemGateway::ShowWidget() {
// 	UpdateQuestNode->ShowWidget();
// }

// void UQuestItemGateway::UpdateQuest() {
// 	QuestStepTextBlock->SetColorAndOpacity(ColorAndOpacity);
// 	QuestStepTextBlock->SetText(FText::FromString(QuestMessage));
// 	UIWidget->UnbindAllFromAnimationFinished(UIWidget->QuestTransition);
// 	UIWidget->PlayAnimation(UIWidget->QuestTransition, 0, 1, EUMGSequencePlayMode::Reverse, 1.3);
// 	Inventory->ItemChangedDelegate.RemoveAll(this);
// 	CurrentState = &UQuestItemGateway::End;
// }

void UQuestItemGateway::EvaluateCondition() {
	if (Inventory) {
		InventoryConditionMet = Inventory->CheckInventory(WhenInventoryItems);
    	if (InventoryConditionMet) {
			UE_LOG(LogTemp, Warning, TEXT("QuestItem in inventory %s"), *WhenInventoryItems);
			if (UpdateQuestNode) {
				UpdateQuestNode->QuestStepText = QuestStepText;
				UpdateQuestNode->NextNode = NextNode;
			} else {
				UE_LOG(LogTemp, Warning, TEXT("QuestItemGateway: UpdateQuestNode is null!"));
			}
			NextNode = UpdateQuestNode;
			// Inventory->ItemChangedDelegate.Remove(this, "EvaluateCondition");
    	}
	}
	CurrentState = &UQuestItemGateway::End;
}

// void UQuestItemGateway::HideWidget() {
//     // if (UIWidget) UIWidget->RemoveFromParent();
// }

// void UQuestItemGateway::WaitShowWidgetTime() {
//     CurrentWaitingTime += GetWorld()->GetDeltaSeconds();
// 	if (CurrentWaitingTime > ShowWidgetTime) {
// 		CurrentWaitingTime = 0;
// 		CurrentState =  &UQuestItemGateway::HideWidget;
// 	}
// }

bool UQuestItemGateway::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UQuestItemGateway::End;
}