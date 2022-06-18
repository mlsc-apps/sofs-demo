// Fill out your copyright notice in the Description page of Project Settings.


#include "IndexGateway.h"

void UIndexGateway::InitNode() {
    if (NextNode) OriginalNextNode = NextNode;
	CurrentState = &UIndexGateway::EvaluateCondition;
}

void UIndexGateway::EvaluateCondition() {
    CurrentCount++;
    UE_LOG(LogTemp, Warning, TEXT("UIndexGateway: Current Count %d"), CurrentCount);
    NextNode = (CurrentCount < MaximumCount) ? nullptr : OriginalNextNode;
    CurrentState = &UIndexGateway::End;
}

bool UIndexGateway::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UIndexGateway::End;
}