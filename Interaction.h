// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "InteractionNodes/Fork.h"
#include "InteractionNodes/Choice.h"
#include "InteractionNodes/SimpleFork.h"
#include "MovieSceneSequencePlayer.h"
#include "Interaction.generated.h"

class UInteractable;
class ULevelSequence;

enum class EFinalizeWith : uint8;


USTRUCT(BlueprintType)
struct FMUKAInteractionSpec : public FTableRowBase {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString Tag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString ActionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString WhenInventoryItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MUKA")
	FString AddInventoryItemsWhenFinished;

};

UCLASS()
class MUKA_API UInteraction : public UObject
{
	GENERATED_BODY()

private:
	UInteractionNode* CurrentNode;
	float TotalWaitTime = 0;
	int nodeIndex = 0;
	float WaitBeforeTimer = 0;
	float MaxWaitBefore;

public:
	// Sets default values for this component's properties
	// UInteraction();

    UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	FString Action;

    UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	bool Conditional = false;
	
    UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	FString Name;
	
	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	bool RunOnPlay;
	
	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	bool Showable = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MUKA|Interaction")
	bool Auto = false;

	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	bool BlockInputOnExecution = true;

	// UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	// TArray<FString> InventoryWhiteList;

	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	FString WhenInventoryItems;

	// UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	// FString InventoryItemsWhenFinished;
	
	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	TArray<UInteractionNode*> InteractionNodes;
	
	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	EFinalizeWith FinalizeWith;

	AActor* Menu;
	// UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	// ACharacter* Vincent;
	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	UInteractable* ParentInteractable;

	UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	AActor* ParentActor;

	// UPROPERTY(EditAnywhere, Category = "MUKA|Interaction")
	// UInteraction* ReplaceInteraction;
	
	protected:
		// virtual void BeginPlay() override;

	private:
	    void InitWithCurrentNode(UInteractionNode* node); 
		void FinalizeNode(UInteractionNode* node, UInteractionNode* previousNode, UInteraction*& interaction, UInteractionNode*& thisNode, float waitBefore = 0, bool active = true, bool AddSpecItemsToInventory = false);
	    bool MoveToNextNode();

	public:
		virtual void Tick(float DeltaTime);

		UInteractionNode* GetInteractionNodeByName(FString NNode);

		FName QuestSlotName;

		UFUNCTION(BlueprintCallable)
		void Reset();

		UFUNCTION(BlueprintCallable)
		void Init(TArray<UInteractionNode*> nodes);

		UFUNCTION(BlueprintCallable)
		void Run();

		// UFUNCTION(BlueprintCallable)
		UInteractionNode* UpdateNextNode(UInteractionNode* NextInteractionNode);
};
