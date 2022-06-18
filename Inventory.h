// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionNodes/Use.h"
#include "Inventory.generated.h"

// class FMUKAInventoryItemSpec;

USTRUCT(BlueprintType)
struct FMUKAInventoryItemSpec : public FTableRowBase {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FName ActorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString Description;
};

USTRUCT(BlueprintType)
struct FMUKAInventoryItem {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FName ActorTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	AActor* Actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	UTexture2D* ItemTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FText Description;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemChangedDelegate); //, const struct FMUKAInventoryItem, Item);

UCLASS()
class MUKA_API AInventory : public AActor {

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

private:
    UUse* UseNode;

public:	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	TMap<FString, FMUKAInventoryItem> Items;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	TSubclassOf<class UUserWidget> InventoryWidget;

	UPROPERTY(EditAnywhere, Category = "MUKA")
	UUserWidget* UIWidget;

	UPROPERTY(BlueprintAssignable)
	FOnItemChangedDelegate ItemChangedDelegate;

	// void AddToInventory(FMUKAInventoryItem Item);
	void RemoveFromInventory(FString Name);
	void OpenInventoryUI(UUse* UseNode);
	bool IsInventoryUIVisible();
	bool IsInInventory(FString ItemLabel);
	bool CheckInventory(FString Query);
	AActor* GetActorFromInventoryItem(FString Name);

	UFUNCTION(BlueprintCallable, Category = "MUKA")
	void ItemChosen(FString ItemName);

	UFUNCTION(BlueprintCallable, Category = "MUKA")
	TArray<FMUKAInventoryItem> GetItems();

	UFUNCTION(BlueprintCallable, Category = "MUKA")
	void AddToInventory(FMUKAInventoryItem Item);
};
