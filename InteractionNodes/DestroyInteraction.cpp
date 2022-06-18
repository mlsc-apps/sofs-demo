// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyInteraction.h"
#include "../Interaction.h"
#include "../Interactable.h"

void UDestroyInteraction::InitNode() {
	CurrentState = &UDestroyInteraction::StartDestroying;
}

void UDestroyInteraction::StartDestroying() {
    ParentInteraction->FinalizeWith = EFinalizeWith::Destroy;
	CurrentState = &UDestroyInteraction::End;
}

bool UDestroyInteraction::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UDestroyInteraction::End;
}