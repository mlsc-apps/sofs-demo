// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
// #include "Blueprint/WidgetBlueprintLibrary.h"

#define NL  "\n"
#define AND "&"
#define OR  "|"
#define NOT "!"

// Sets default values
AInventory::AInventory() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AInventory::AddToInventory(FMUKAInventoryItem Item) {
	Items.Add(Item.Name, Item);
    ItemChangedDelegate.Broadcast();
}

void AInventory::RemoveFromInventory(FString Name) {
	TArray<FString> ItemList;
	bool Result = true;
	Name.ParseIntoArray(ItemList, TEXT(NL), false);
	for (auto &&Item : ItemList) {
		Item = Item.TrimEnd();
		Items.Remove(Item);
	}
	ItemChangedDelegate.Broadcast();
}

bool AInventory::IsInInventory(FString ItemName) {
	return Items.Contains(ItemName);
}

bool AInventory::CheckInventory(FString Query) {
	TArray<FString> QueryLines;
	bool Result = true;
	Query.ParseIntoArray(QueryLines, TEXT(NL), false);
	UE_LOG(LogTemp, Warning, TEXT("AInventory: Evaluate Query %s"), *Query);
	for (auto QueryLine : QueryLines) {
		QueryLine = QueryLine.TrimEnd();
		// UE_LOG(LogTemp, Warning, TEXT("AInventory: Evaluate QueryLine %s"), *QueryLine);
		if (QueryLine.StartsWith(AND)) {
			QueryLine.RemoveFromStart(AND);
			// UE_LOG(LogTemp, Warning, TEXT("AInventory: Evaluate QueryLine %s"), *QueryLine);
			Result = QueryLine.StartsWith(NOT) ? (Result && !IsInInventory(QueryLine)) : (Result && IsInInventory(QueryLine));
		} else if (QueryLine.StartsWith(OR)) {
			QueryLine.RemoveFromStart(OR);
			Result = Result || IsInInventory(QueryLine);
		} else if (QueryLine.StartsWith(NOT)) {
			QueryLine.RemoveFromStart(NOT);
			Result = !IsInInventory(QueryLine);
		} else {
			Result = IsInInventory(QueryLine);
		}
		// UE_LOG(LogTemp, Warning, TEXT("AInventory: Result: %s"), (Result ? TEXT("True") : TEXT("False")));
	}

	return Result;
}

void AInventory::OpenInventoryUI(UUse* UNode) {
	UseNode = UNode;
    // TArray<UUserWidget*> FoundWidgets;
    // TSubclassOf< UUserWidget > WidgetClass,
	// UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, InventoryWidget);
	// for (auto &&Widget : FoundWidgets) {
	// 	Widget->RemoveFromParent();
	// }
	
	UIWidget = CreateWidget<UUserWidget>(GetGameInstance(), InventoryWidget);
	if (UIWidget) UIWidget->AddToViewport();
}

bool AInventory::IsInventoryUIVisible() {
	return UIWidget ? UIWidget->IsInViewport() : false;
}

void AInventory::ItemChosen(FString ItemName) {
	if (UseNode) {
		UseNode->SetItemChosen(ItemName);
	}
	if (UIWidget) {
		UIWidget->RemoveFromParent();
		// GEngine->PerformGarbageCollectionAndCleanupActors();
	}
	UseNode = nullptr;
}

TArray<FMUKAInventoryItem> AInventory::GetItems() {
	TArray<FMUKAInventoryItem> Values;
	Items.GenerateValueArray(Values);
	return Values;
}

AActor* AInventory::GetActorFromInventoryItem(FString ItemName) {
	if (Items.Contains(ItemName)) {
		return Items[ItemName].Actor;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Inventory: Item with name %s not found in inventory"), *ItemName);
	}
	return nullptr;
}