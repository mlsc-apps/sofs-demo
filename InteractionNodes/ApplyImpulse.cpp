// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyImpulse.h"
#include "Components/StaticMeshComponent.h"
#include "../Interactable.h"
#include "../Interaction.h"
#include "Kismet/GameplayStatics.h"

class UStaticMeshComponent;
class AInteraction;

void UApplyImpulse::InitNode() {
	CurrentState = &UApplyImpulse::StartApplyingImpulse;
}

void UApplyImpulse::StartApplyingImpulse() {
    if (ActorTag == NAME_None) {
        Target = ParentInteraction->ParentInteractable->GetOwner();
    }
    if (ActorTag != NAME_None) {
        Target = GetActorByTag(ActorTag);
    }
    
    if (Target) {
            UStaticMeshComponent* mesh = Target->FindComponentByClass<UStaticMeshComponent>();
            if (mesh) {
                UE_LOG(LogTemp, Warning, TEXT("UApplyImpulse: Adding impuls %s to actor: %s"), *Force.ToString(), *ActorTag.ToString());
                mesh->SetSimulatePhysics(true);
                mesh->AddImpulseAtLocation(Force, mesh->GetComponentLocation() + ForceLocation);
                if (Sound) {
                    UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Target->GetActorLocation());      
                }
            } else {
                UE_LOG(LogTemp, Warning, TEXT("UApplyImpulse: Cannot find mesh component to add impulse to for actor: %s"), *Target->GetName());
            }
        } else {
            UE_LOG(LogTemp, Warning, TEXT("UApplyImpulse: Cannot find Target actor with Tag: %s"), *ActorTag.ToString());
        }
	CurrentState = &UApplyImpulse::End;
}

bool UApplyImpulse::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UApplyImpulse::End;
} 