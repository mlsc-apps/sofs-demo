// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction.h"
#include "Kismet/GameplayStatics.h"
#include "InteractionNodes/Monologue.h"
#include "InteractionNodes/ReplaceInteractions.h"
#include "InteractionNodes/ApplyImpulse.h"
#include "InteractionNodes/BlendToCamera.h"
#include "InteractionNodes/Wait.h"
#include "InteractionNodes/Destroy.h"
#include "InteractionNodes/AddInventory.h"
#include "InteractionNodes/Fork.h"
#include "InteractionNodes/Branch.h"
#include "InteractionNodes/Conversation.h"
#include "InteractionNodes/PlaySequence.h"
#include "InteractionNodes/Choice.h"
#include "InteractionNodes/DestroyInteraction.h"
#include "Math/UnrealMathUtility.h"
#include "Interactable.h"

bool UInteraction::MoveToNextNode() {
	if (CurrentNode->NextNode) {
		CurrentNode = CurrentNode->NextNode;
		// UE_LOG(LogTemp, Warning, TEXT("Current Node changed to: %s"), CurrentNode->NextNode);
		if (!CurrentNode->Active) {
			return MoveToNextNode();
		} else {
			InitWithCurrentNode(CurrentNode);
		    return true;
		}
	}
	return false;
}

// Called every frame
void UInteraction::Tick(float DeltaTime) {
	if (!CurrentNode) return;
	if (WaitBeforeTimer < MaxWaitBefore) {
		WaitBeforeTimer += GetWorld()->GetDeltaSeconds();
		return;
	}
	bool nodeEnded = CurrentNode->Exec();
	if (nodeEnded) { 
				bool NextNodeExists = MoveToNextNode();
				if (!NextNodeExists) {
					if (BlockInputOnExecution) {
						GetWorld()->GetFirstPlayerController()->bEnableMouseOverEvents = true;
						GetWorld()->GetFirstPlayerController()->bEnableClickEvents = true;
					}
					if (ParentInteractable) {
						ParentInteractable->CurrentInteraction = NULL;
					}
					if (FinalizeWith == EFinalizeWith::Destroy) {
						if (ParentInteractable) ParentInteractable->Interactions.Remove(Name);
					}
				}
	}
}

void UInteraction::FinalizeNode(UInteractionNode* node, UInteractionNode* previousNode, UInteraction*& interaction, UInteractionNode*& thisNode, float waitBefore, bool active, bool AddSpecItemsToInventory) {
}

void UInteraction::Reset() {
	InteractionNodes.Empty();
}

void UInteraction::Init(TArray<UInteractionNode*> nodes) {
	InteractionNodes = nodes;
}

void UInteraction::Run() {
	if (InteractionNodes.Num() > 0) {
		if (Menu) {
			Menu->SetActorHiddenInGame(true);
		}
		CurrentNode = InteractionNodes[0];
		if (CurrentNode == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Interaction without Nodes!"));
		} else {
			if (ParentInteractable) ParentInteractable->CurrentInteraction = this;
			InitWithCurrentNode(CurrentNode);
		}

	}

}

void UInteraction::InitWithCurrentNode(UInteractionNode* InitCurrentNode) {
	if (InitCurrentNode) {
		InitCurrentNode->InitNode();
		MaxWaitBefore = InitCurrentNode->WaitBefore;
		WaitBeforeTimer = 0;
		GetWorld()->GetFirstPlayerController()->bEnableMouseOverEvents = !InitCurrentNode->BlockInput;
		GetWorld()->GetFirstPlayerController()->bEnableClickEvents = !InitCurrentNode->BlockInput;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Current Node for running Interaction is null!"));
	}
}

UInteractionNode* UInteraction::GetInteractionNodeByName(FString FindNName) {
	for (auto &&InteractionNode : InteractionNodes) {
		if (InteractionNode->NName == FindNName) {
			return InteractionNode;
		}
	}
	return nullptr;
}

UInteractionNode* UInteraction::UpdateNextNode(UInteractionNode* NextInteractionNode) {
	CurrentNode->NextNode = NextInteractionNode;
	return CurrentNode;
}
