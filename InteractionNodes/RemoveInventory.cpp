// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoveInventory.h"
#include "../Inventory.h"
#include "../Interactable.h"

void URemoveInventory::InitNode() {
	CurrentState = &URemoveInventory::StartingRemovingFromInventory;
}

void URemoveInventory::StartingRemovingFromInventory() {
    AInventory* Inventory = GetInventory();
    if (Inventory) {
        AActor* ItemActor = Inventory->GetActorFromInventoryItem(Name);
        if (ItemActor) {
            UInteractable* Interactable = ItemActor->FindComponentByClass<UInteractable>();
		    if (Interactable) Interactable->Active = true;
        }
        Inventory->RemoveFromInventory(Name);
    }
    CurrentState = &URemoveInventory::End;
}

bool URemoveInventory::Exec() {
	(this->*CurrentState)();
	return CurrentState == &URemoveInventory::End;
} 