// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayAnimation.h"
#include "../Interaction.h"
#include "../Interactable.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"

void UPlayAnimation::InitNode() {
	AnimationActor = ActorTag.Len() == 0 ? Cast<AActor>(ParentInteraction->ParentInteractable->GetOwner()) : GetActorByTag(FName(ActorTag));
	if (AnimationActor) {
		Mesh = AnimationActor->FindComponentByClass<USkeletalMeshComponent>();
		CurrentState = Animation ? &UPlayAnimation::StartPlayingAnimation : &UPlayAnimation::StartPlayingMontage;
	} else {
		 UE_LOG(LogTemp, Error, TEXT("UPlayAnimation: AnimationActor Not Found for Animation"));
		 CurrentState = &UPlayAnimation::End;
	}
}

bool UPlayAnimation::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UPlayAnimation::End;
}

void UPlayAnimation::StartPlayingMontage() {
	if (Mesh) {
		MontageLengthInSeconds = Mesh->GetAnimInstance()->Montage_Play(Montage);
		WaitCallback =  &UPlayAnimation::End;
		CurrentState = WaitUntilEnded ? &UPlayAnimation::WaitingUntilMontageEnded : &UPlayAnimation::End;
	} else {
		UE_LOG(LogTemp, Error, TEXT("UPlayAnimation: AnimationActor does not have USkeletalMeshComponent"));
		CurrentState = &UPlayAnimation::End;
	}
}

void UPlayAnimation::StartPlayingAnimation() {
	if (Mesh) {
		Mesh->PlayAnimation(Animation, Loop);
		CurrentState = WaitUntilEnded ? &UPlayAnimation::WaitingUntilAnimationEnded : &UPlayAnimation::End;
	} else {
		UE_LOG(LogTemp, Error, TEXT("UPlayAnimation: AnimationActor does not have USkeletalMeshComponent"));
		CurrentState = &UPlayAnimation::End;
	}
}

void UPlayAnimation::WaitingUntilAnimationEnded() {
	if (Mesh->IsPlaying()) return;
	CurrentState = &UPlayAnimation::End;
}

void UPlayAnimation::WaitingUntilMontageEnded() {
	CurrentWaitingTime += GetWorld()->GetDeltaSeconds();
	if (CurrentWaitingTime > MontageLengthInSeconds) {
		CurrentWaitingTime = 0;
		CurrentState = WaitCallback;
	}
}