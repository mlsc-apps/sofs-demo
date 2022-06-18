// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivateInteractable.h"
#include "../Interaction.h"
#include "../Interactable.h"

void UActivateInteractable::InitNode() {
	CurrentState = &UActivateInteractable::StartChangingState;
}

void UActivateInteractable::StartChangingState() {
	AActor* InteractableOwner = GetActorByTag(ActorTag);
	if (InteractableOwner) {
		UInteractable* Interactable = InteractableOwner->FindComponentByClass<UInteractable>();
		if (Interactable) {
			Interactable->Active = Activate;
		} else {
			UE_LOG(LogTemp, Warning, TEXT("UActivateInteractable: Interactable Component for Actor %s not found"), *ActorTag.ToString());
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("UActivateInteractable: Actor %s not found"), *ActorTag.ToString());
	}
	CurrentState = &UActivateInteractable::End;
}

bool UActivateInteractable::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UActivateInteractable::End;
} 