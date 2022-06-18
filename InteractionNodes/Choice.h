// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "Choice.generated.h"

class UProgressBar;
class UVerticalBox;
class AInventory;
class UBranch;

// USTRUCT(BlueprintType)
// struct FChoiceOption {

// 	GENERATED_BODY()

//     // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Choice")
// 	// FString Name;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Choice")
// 	FString Text;
//     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Choice")
// 	FString BranchName;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Choice")
// 	FString InventoryItem;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Choice")
// 	bool RunOnce = false;

// };

/**
 * 
 */
UCLASS()
class MUKA_API UChoice : public UInteractionNode {
	
	GENERATED_BODY()

// public:
//    UChoice();

private:
//    AActor* ContextChoice;
   UUserWidget* ChoiceWidget;
   UProgressBar* ChoiceTimeoutBar;
   float Timeout = 0;
   float Elapsed = 0;
   UBranch* TimeoutNode;
   UVerticalBox* ChoicesVerticalBox;
   AInventory* Inventory;

public:
	typedef void (UChoice::* Action)(void);
	Action CurrentState;
	
public:
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // TArray<FChoiceOption> Choices;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // FString ChoiceName;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // TSubclassOf<class UUserWidget> ContextChoiceBP;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// UUserWidget* ContextChoiceWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Speaker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, UInteractionNode*> NodesMap;

	void InitNode() override;
	bool Exec() override;

	void StartChoices();
	void ChangeCamera();
	void WaitForChoice();
	void WaitForChoiceWithTimeout();
	void UpdateInteractionNextNode();
	// void HideChoicesBox();
	// void RemoveOptionAtIndex(int32 index);
	// UFUNCTION()
	// void Bye();

	void SetNodeAndFinish(UBranch* Node);
	
};
