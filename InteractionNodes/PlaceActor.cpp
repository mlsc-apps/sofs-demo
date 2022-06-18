// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceActor.h"


void UPlaceActor::InitNode() {
	CurrentState = &UPlaceActor::StartPlacingActor;
}

bool UPlaceActor::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UPlaceActor::End;
}

void UPlaceActor::StartPlacingActor() {
    if (ActorTag != NAME_None) {
        AActor* Actor = GetActorByTag(ActorTag);
        if (Actor) {
            AActor* Target = GetActorByTag(TargetTag);
            FVector NewLocation;
            if (Target) {
                NewLocation = Target->GetActorLocation() + Location;
            } else {
                NewLocation = Location;
            }
            Actor->SetActorLocation(NewLocation);
            Actor->SetActorHiddenInGame(false);
        } 
    }
    CurrentState = &UPlaceActor::End;
}