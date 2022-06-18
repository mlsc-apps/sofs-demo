// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../InventoryItem.h"
#include "InteractionNode.generated.h"

class UInteraction;
class AInventory;

/**
 * 
 */
UCLASS()
class MUKA_API UInteractionNode : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	FString NName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	bool Active;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	float WaitBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	bool BlockInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	UInteractionNode* NextNode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	UInteraction* ParentInteraction;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	// bool AddSpecItemsToInventory;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction Node")
	// UInteractionNode* VirtualNextNode;

	virtual void InitNode();
	virtual bool Exec();
	void End();

protected:
    UInteraction* GetInteractionByName(AActor* InteractionOwner, FString Tag);
	AActor* GetActorByTag(FName tag);
	TArray<AActor*> GetAllActorsByTag(FName tag);
	AInventory* GetInventory();
	// bool InventoryWhiteListMet(TArray<FString> WhiteList);
	// bool InventoryBlackListMet(TArray<FString> BlackList);
	// void UpdateInteractionNextNode();
};
