// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGateway.h"
#include "../Inventory.h"

void UItemGateway::InitNode() {
    // WaitCallback = &UItemGateway::EvaluateCondition;
	if (!Inventory) {
		Inventory = GetInventory();
	}
	if (Inventory) {
		Inventory->ItemChangedDelegate.AddDynamic(this, &UItemGateway::EvaluateCondition);
	}
	CurrentState = &UItemGateway::Wait;
}

void UItemGateway::Wait() {
	// CurrentWaitingTime += GetWorld()->GetDeltaSeconds();
	// if (CurrentWaitingTime > Interval) {
	// 	CurrentWaitingTime = 0;
	// 	CurrentState = WaitCallback;
	// }
}

void UItemGateway::EvaluateCondition() {
	if (Inventory) {
		// UE_LOG(LogTemp, Warning, TEXT("Evaulate Interaction condition"));
		bool InventoryConditionMet = Inventory->CheckInventory(WhenInventoryItems);
    	if (InventoryConditionMet) {
			Inventory->ItemChangedDelegate.RemoveAll(this);
        	CurrentState =  &UItemGateway::End;
    	}
	}
}

bool UItemGateway::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UItemGateway::End;
}