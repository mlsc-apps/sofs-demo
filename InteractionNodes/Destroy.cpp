// Fill out your copyright notice in the Description page of Project Settings.


#include "Destroy.h"
// #include "Kismet/GameplayStatics.h"

void UDestroy::InitNode() {
	CurrentState = &UDestroy::StartDestroying;
}

void UDestroy::StartDestroying() {
    TArray<AActor *> FoundActors = GetAllActorsByTag(Tag);
	if (FoundActors.Num() > 0) {
		// ContextLabel = FoundActors[0];
        for (auto &FoundActor : FoundActors) {
            FoundActor->Destroy();
        }
    }
	CurrentState = &UDestroy::End;
}

bool UDestroy::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UDestroy::End;
}