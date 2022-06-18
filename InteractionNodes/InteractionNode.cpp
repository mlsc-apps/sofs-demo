// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionNode.h"
#include "Kismet/GameplayStatics.h"
#include "../Interactable.h"
#include "../Interaction.h"
#include "../Inventory.h"
#include "Branch.h"

// class UInteractable;
class UInteraction;
// class AInventory;

void UInteractionNode::End() {
}

void UInteractionNode::InitNode() {
}

bool UInteractionNode::Exec() {
	return false;
}

UInteraction* UInteractionNode::GetInteractionByName(AActor* InteractionOwner, FString Name) {
		UInteractable* interactable = InteractionOwner->FindComponentByClass<UInteractable>();
		if (interactable) {
			TMap<FString, UInteraction*> interactions = interactable->Interactions;
			if (interactions.Contains(Name)) {
				return interactions[Name];
			}
		} else {
			UE_LOG(LogTemp, Warning, TEXT("UInteractionNode: Actor Interaction Owner does not have interactable component!"));
		}
		return nullptr;
}

AActor* UInteractionNode::GetActorByTag(FName Tag) {
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);
    if (FoundActors.Num() > 0) {
		return FoundActors[0];
	}
	return nullptr;
}

TArray<AActor*> UInteractionNode::GetAllActorsByTag(FName Tag) {
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);
    if (FoundActors.Num() > 0) {
		return FoundActors;
	}
	return FoundActors;
}

AInventory* UInteractionNode::GetInventory() {
	return Cast<AInventory>(GetActorByTag(TEXT("Inventory")));
}

// bool UInteractionNode::InventoryWhiteListMet(TArray<FString> ItemList) {
// 	    if (ItemList.Num() == 0) {
// 			return true;
// 		}
// 	    bool AllItemsPresent = false;
// 		// TArray<FString> Items;
//         // ItemList.ParseIntoArray(Items, TEXT("\n"), false);
// 	    for (auto &&Item : ItemList) {
//             // Item = Item.TrimEnd();
//             AllItemsPresent = GetInventory()->IsInInventory(Item); //Items.Contains(Item);
//             if (!AllItemsPresent) break;
//         }
//         return AllItemsPresent;   
// }

// bool UInteractionNode::InventoryBlackListMet(TArray<FString> ItemList) {
// 		if (ItemList.Num() == 0) {
// 			return true;
// 		}
// 	    bool AllItemsAbsent = false;
// 		// TArray<FString> Items;
//         // ItemList.ParseIntoArray(Items, TEXT("\n"), false);
// 	    for (auto &&Item : ItemList) {
//             // Item = Item.TrimEnd();
//             AllItemsAbsent = !GetInventory()->IsInInventory(Item); //.Contains(Item);
//             if (!AllItemsAbsent) break;
//         }
//         return AllItemsAbsent;   
// }