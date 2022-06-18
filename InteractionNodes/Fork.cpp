// Fill out your copyright notice in the Description page of Project Settings.


#include "Fork.h"
#include "../Inventory.h"
#include "../Interaction.h"
#include "Branch.h"

void UFork::InitNode() {
	CurrentState = &UFork::StartForking;
    Inventory = GetInventory();
}

void UFork::StartForking() {
   
    NextNode = nullptr;

    for (auto &&Node : NodesMap) {
        UBranch* Branch = Cast<UBranch>(Node.Value);
        if (Branch && Inventory) {
            if (Branch->BranchName == "Default") continue;
            bool Condition = Inventory->CheckInventory(Branch->WhenInventoryItems);
            if (Condition) {
                if (NextNode) UE_LOG(LogTemp, Warning, TEXT("UFork: Two Branches evaluated to True. Overriding Branch with %s"), *Node.Key);
                NextNode = Branch;
            }
        }
        if (!NextNode && NodesMap.Contains("Default")) NextNode = NodesMap["Default"];
        UFork::CurrentState = &UFork::End;
    }
    
    // if (Inventory) {
    //     bool Condition = Inventory->CheckInventory(WhenInventoryItems);
    //     if (Condition && NodesMap.Contains("True")) {
    //         NextNode = NodesMap["True"];
    //         UE_LOG(LogTemp, Warning, TEXT("UFork: Evaluated to True"));
    //     }
    //     if (!Condition && NodesMap.Contains("False")) {
    //         NextNode = NodesMap["False"];
    //         UE_LOG(LogTemp, Warning, TEXT("UFork: Evaluated to False"));
    //     }
    // }
    // if (!NextNode) {
    //     NextNode = ParentInteraction->GetInteractionNodeByName(DefaultBranch);
    //     UE_LOG(LogTemp, Warning, TEXT("UFork: Did not find Valid Conditional Path. Using Default node: %s"), *DefaultBranch);
    // }
    // if (!NextNode) {
    //     UE_LOG(LogTemp, Warning, TEXT("UFork: Fork Did Not find a Path to continue! Interaction terminates here"));
    // }
    UFork::CurrentState = &UFork::End;
}

bool UFork::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UFork::End;
}