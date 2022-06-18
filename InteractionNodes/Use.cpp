// Fill out your copyright notice in the Description page of Project Settings.


#include "Use.h"
#include "../Interaction.h"
#include "../Inventory.h"

void UUse::InitNode() {
	CurrentState = &UUse::OpenInventoryWidget;
}

bool UUse::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UUse::End;
}

void UUse::WaitForPlayerChoice() {
	if (Inventory && !Inventory->IsInventoryUIVisible()) {
		UE_LOG(LogTemp, Warning, TEXT("Detected user closing UI Widget. Interaction will exit"));
		NextNode = nullptr;
		CurrentState = &UUse::End;
	}
}

void UUse::OpenInventoryWidget() {
	Inventory = GetInventory();
	if (Inventory) {
		Inventory->OpenInventoryUI(this);
		CurrentState = &UUse::WaitForPlayerChoice;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Inventory UI Widget not found"));
		CurrentState = &UUse::End;
	}
	
}

void UUse::SetItemChosen(FString ItemName) {
	UE_LOG(LogTemp, Warning, TEXT("Item Chosen: %s"), *ItemName);
	if (NodesMap.Contains(ItemName)) {
		UInteractionNode* NextChosenNode = NodesMap[ItemName];
		NextNode = NextChosenNode;
	}
	CurrentState = &UUse::End;
}