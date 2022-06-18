// Fill out your copyright notice in the Description page of Project Settings.


#include "CompositeItemGateway.h"
#include "../Inventory.h"
#include "Animation/UMGSequencePlayer.h"

void UCompositeItemGateway::InitNode() {
	if (!Inventory) {
		Inventory = GetInventory();
	}
	if (Inventory) {
		Inventory->ItemChangedDelegate.AddDynamic(this, &UCompositeItemGateway::OnInventoryUpdate);
		// UE_LOG(LogTemp, Warning, TEXT("UQuestItemGateway Registered"));
	}
	CurrentState = &UCompositeItemGateway::Wait;
}

void UCompositeItemGateway::Wait() {
}

void UCompositeItemGateway::OnInventoryUpdate() {
	if (Inventory) {
		FString KeyFound;
		UE_LOG(LogTemp, Warning, TEXT("UCompositeItemGateway: Inventory Update Received"));
		for (auto &&NodeMapEntry : NodesMap) {
			bool NodeKeyExists = Inventory->CheckInventory(NodeMapEntry.Key);
			if (NodeKeyExists) {
				UE_LOG(LogTemp, Warning, TEXT("UCompositeItemGateway: NodeItem in inventory %s"), *NodeMapEntry.Key);
				// QuestMessage = TransientStep.Value;
				KeyFound = NodeMapEntry.Key;
				NextNode = NodeMapEntry.Value;
				break;
			}
		}
		if (KeyFound.Len() > 0) {
			NodesMap.Remove(KeyFound);
			Inventory->ItemChangedDelegate.RemoveAll(this);
			CurrentState =  &UCompositeItemGateway::End;
		} else {
			CurrentState =  &UCompositeItemGateway::Wait;
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("UCompositeItemGateway: Inventory Not Found."));
		CurrentState = &UCompositeItemGateway::End;
	}
}

bool UCompositeItemGateway::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UCompositeItemGateway::End;
}