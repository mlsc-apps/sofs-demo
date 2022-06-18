// Fill out your copyright notice in the Description page of Project Settings.


#include "Branch.h"
#include "../Interaction.h"
#include "Choice.h"

void UBranch::InitNode() {
	CurrentState = &UBranch::End;
}

bool UBranch::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UBranch::End;
}

void UBranch::UpdateConversationChoiceNextNode() {
	// UInteractionNode* CurrentNode = ParentInteraction->UpdateNextNode(this);
	// UChoice* Choice = Cast<UChoice>(CurrentNode);
	// if (Choice) {
	// 	if (RunOnce) {
	// 		Choice->RemoveOptionAtIndex(BranchIndex);
	// 	}
	// 	Choice->HideChoicesBox();
	// 	Choice->CurrentState = &UChoice::End;
	// }
	ConversationChoice->SetNodeAndFinish(this);
}