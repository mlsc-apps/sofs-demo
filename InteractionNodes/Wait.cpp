// Fill out your copyright notice in the Description page of Project Settings.


#include "Wait.h"

void UWait::InitNode() {
	CurrentState = &UWait::Wait;
}

// void UWait::StartWaiting() {
//     if (Target) {
//         GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Target, BlendTime, EViewTargetBlendFunction::VTBlend_Linear);
//         UE_LOG(LogTemp, Warning, TEXT("UWait: Blending to Camera %s"), *Target->GetName());
//     }
// 	CurrentState = &UWait::End;
// }

bool UWait::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UWait::End;
}

void UWait::Wait() {
	CurrentWaitingTime += GetWorld()->GetDeltaSeconds();
	if (CurrentWaitingTime > WaitTime) {
		UE_LOG(LogTemp, Warning, TEXT("Elapsed %f"), CurrentWaitingTime);
		CurrentWaitingTime = 0;
		CurrentState = &UWait::End;
	}
}