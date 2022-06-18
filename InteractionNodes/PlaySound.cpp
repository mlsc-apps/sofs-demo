// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaySound.h"
#include "Kismet/GameplayStatics.h"

void UPlaySound::InitNode() {
	CurrentState = &UPlaySound::StartPlayingSound;
}

bool UPlaySound::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UPlaySound::End;
}

void UPlaySound::StartPlayingSound() {
    UGameplayStatics::PlaySound2D(GetWorld(), Sound);
    CurrentState = &UPlaySound::End;
}