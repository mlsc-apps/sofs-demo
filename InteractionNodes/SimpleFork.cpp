// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFork.h"
#include "../Inventory.h"
#include "../Interaction.h"

void USimpleFork::InitNode() {
	CurrentState = &USimpleFork::StartForking;
    UE_LOG(LogTemp, Warning, TEXT("USimpleFork Init Forking"));
}

void USimpleFork::StartForking() {
    AInventory* Inventory = GetInventory();
    NextNode = NULL;
    if (Inventory) {
        if (Inventory->IsInInventory(Condition.Item)) {
            NextNode = ParentInteraction->GetInteractionNodeByName(Condition.BranchName);
        }
    }
    if (!NextNode) {
        NextNode = ParentInteraction->GetInteractionNodeByName(DefaultBranch);
        UE_LOG(LogTemp, Warning, TEXT("USimpleFork: Did not find Valid Conditional Path. Using Default node: %s"), *DefaultBranch);
    }
    if (!NextNode) {
        UE_LOG(LogTemp, Warning, TEXT("USimpleFork: Fork Did Not find a Path to continue! Interaction terminates here"));
    }
    USimpleFork::CurrentState = &USimpleFork::End;
}

bool USimpleFork::Exec() {
	(this->*CurrentState)();
	return CurrentState == &USimpleFork::End;
}