// Fill out your copyright notice in the Description page of Project Settings.


#include "AddInventory.h"
#include "../Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UAddInventory::InitNode() {
	CurrentState = &UAddInventory::StartAddingToInventory;
}

void UAddInventory::StartAddingToInventory() {
    FMUKAInventoryItem Item;
    Item.ActorTag    = ItemSpecification.ActorTag;
    Item.Name        = ItemSpecification.Name;
    Item.Description = FText::FromString(ItemSpecification.Description);
    Item.Actor = nullptr;
	if (ItemSpecification.ActorTag != NAME_None) {
        Item.Actor = GetActorByTag(ItemSpecification.ActorTag);
        if (Item.Actor) {
		        UInteractable* Interactable = Item.Actor->FindComponentByClass<UInteractable>();
		        if (Interactable) {
			        Item.ItemTexture = Interactable->Thumbnail;
                    Interactable->Active = false;
		        }   
                Item.Actor->SetActorHiddenInGame(true);
                // Item.Actor->GetWorld()->RemoveActor(Item.Actor, true);
                // ULevel* PersistentLevel = GetWorld()->GetLevel(0);
                // Item.Actor->Rename((const TCHAR *)0, PersistentLevel); //Set the outer of Actor to NewLevel
                // PersistentLevel->Actors.Add(Item.Actor);
            } else {
                UE_LOG(LogTemp, Warning, TEXT("UAddInventory: Cannot find actor with Tag: %s"), *ItemSpecification.ActorTag.ToString());
            }    
    }

    AInventory* Inventory = GetInventory();
    if (Inventory) {
        Inventory->AddToInventory(Item);
    }
    CurrentState = &UAddInventory::End;
}

bool UAddInventory::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UAddInventory::End;
} 