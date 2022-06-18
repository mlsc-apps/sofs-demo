// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SFSHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MUKA_API USFSHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* QuestTransition;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* QuestTransitionMute;
};
