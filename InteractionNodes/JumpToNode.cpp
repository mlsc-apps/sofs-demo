// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpToNode.h"
#include "../Interaction.h"

void UJumpToNode::InitNode() {
	CurrentState = &UJumpToNode::JumpToNode;
}

void UJumpToNode::JumpToNode() {
	if (JumpNode) {
		NextNode = JumpNode;		 
	} else {
		for (auto &&Node : ParentInteraction->InteractionNodes) {
			if (Node->NName == JumpNodeName) {
				NextNode = Node;
				break;
			}
		}
	}
	if (!NextNode) {
		UE_LOG(LogTemp, Error, TEXT("Error creating JumpToNode node: No jump node defined"));
	}
	
    CurrentState = &UJumpToNode::End;    
}

bool UJumpToNode::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UJumpToNode::End;
}