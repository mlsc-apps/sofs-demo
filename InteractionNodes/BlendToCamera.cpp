// Fill out your copyright notice in the Description page of Project Settings.


#include "BlendToCamera.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
// #include "Kismet/GameplayStatics.h"

void UBlendToCamera::InitNode() {
    if (!Target) {
        Target = GetActorByTag(ActorTag);
        // TArray<AActor *> FoundActors;
        // UGameplayStatics::GetAllActorsWithTag(GetWorld(), ActorTag, FoundActors);
		// if (FoundActors.Num() > 0) {
		// 	Target = FoundActors[0];
		// }
    }
    if (Target) UE_LOG(LogTemp, Warning, TEXT("UBlendToCamera: Init with Camera %s"), *Target->GetName());
    CurrentState = &UBlendToCamera::StartBlendingToCamera;
}

void UBlendToCamera::StartBlendingToCamera() {
    if (Target) {
        GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Target, BlendTime, EViewTargetBlendFunction::VTBlend_Linear);
        UE_LOG(LogTemp, Warning, TEXT("UBlendToCamera: Blending to Camera %s"), *Target->GetName());
    }
	CurrentState = &UBlendToCamera::End;
}

bool UBlendToCamera::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UBlendToCamera::End;
}