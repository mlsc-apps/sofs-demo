// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionNode.h"
#include "SimpleFork.generated.h"

USTRUCT(BlueprintType)
struct FSimpleCondition {

	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Simple Fork Condition")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Simple Fork Condition")
	FString Item;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Simple Fork Condition")
	FString BranchName;

};

/**
 * 
 */
UCLASS()
class MUKA_API USimpleFork : public UInteractionNode
{
	GENERATED_BODY()
	
private:
	typedef void (USimpleFork::* Action)(void);
	Action CurrentState;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSimpleCondition Condition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Simple Fork Condition")
	FString DefaultBranch;
	
	void InitNode() override;
	bool Exec() override;

	void StartForking();
};
