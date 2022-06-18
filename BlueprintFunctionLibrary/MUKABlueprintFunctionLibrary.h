// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Interactable.h"
#include "../Interaction.h"
#include "../Inventory.h"
#include "MUKABlueprintFunctionLibrary.generated.h"

class UInteraction;
class UInteractable;
class UInteractionNode;
class ULevelSequence;
// enum class EFinalizeWith : uint8;

// #define VINCENT "Vincent"
/**
 * 
 */
UCLASS()
class MUKA_API UMUKABlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	private:
		static void FinalizeNode(UInteraction* Interaction, UInteractionNode* ThisNode, UInteractionNode* PreviousNode, bool BlockInput, float WaitBefore, bool Active);
	public:
		UFUNCTION(meta = (BlueprintInternalUseOnly = "true"), BlueprintCallable, Category = "MUKA")
        static UInteraction* AddInteraction(UInteractable* Interactable, FString Action, FString Name, EFinalizeWith FinalizeWith, FString WhenInventoryItems, bool RunOnPlay, bool Auto=false, bool Showable=true);
		
		UFUNCTION(meta = (BlueprintInternalUseOnly = "true"), BlueprintCallable, Category = "MUKA")
		static UInteractionNode* Monologue(UInteraction* Interaction, UInteractionNode* PreviousNode, FString Lines, float Delay = 2.0f, FName Speaker = "Vincent", bool BlockInput = true, float WaitBefore = 0, bool Active = true);

		UFUNCTION(meta = (BlueprintInternalUseOnly = "true"), BlueprintCallable, Category = "MUKA")
		static UInteractionNode* AddToInventory(UInteraction* Interaction, UInteractionNode* PreviousNode, FMUKAInventoryItemSpec InventoryItem, float WaitBefore = 0, bool Active = true);

		
		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void ItemGateway(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString WhenInventoryItems, bool BlockInput = false, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void QuestItemGateway(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString WhenInventoryItems, FString QuestStepDescription, bool BlockInput = false, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void UpdateQuest(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString QuestStepDescription, bool Mute = false, bool BlockInput = false, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void FinishQuest(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, bool Mute = false, bool BlockInput = false, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void CompositeItemGateway(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void PlayAnimation(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString ActorTag, UAnimMontage* Montage, UAnimSequence* Animation, bool Loop, bool WaitUntilEnded = false, bool BlockInput = true, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void PlaySequence(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, ULevelSequence* Sequence, bool WaitUntilEnded = false, bool BlockInput = true, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void ActivateInteractable(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName ActorTag, bool Activate = true, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void Use(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void Branch(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString Option, FString WhenInventoryItems, bool RunOnce = false, float Timeout = 0);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static AInventory* GetInventory(APlayerController* PlayerController);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void ApplyImpulse(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName Actor, FVector Force, FVector ForceLocation, USoundBase* Sound, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void ChangeCamera(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName Actor, float BlendTime, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void Destroy(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName Actors, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void PlaceActor(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName ActorTag, FName TargetTag, FVector Location, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void PlaySound(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, USoundBase* Sound, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void RemoveFromInventory(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString InventoryItemName, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void IndexGateway(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, int MaximumCount, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void JumpToNode(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode,  FString NextNodeName, UInteractionNode* NextNode, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void Fork(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString NodeName);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void ConversationChoice(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString NodeName, FName Speaker = "Vincent", FName Camera = "ConversationCamera1", float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void Conversation(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName SpeakerToShow, FName Speaker0 = "Vincent", FName Speaker0Camera = "ConversationCamera1", FName Speaker1 = "None", FName Speaker1Camera = "ConversationCamera1", FString Script = "", bool ChangeCameras = true, bool ShowSpeakers = false, float DelayBetweenLines = 2, float DelayBetweenSpeakers = 1, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void ConversationWithSpeech(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName SpeakerToShow, TArray<USoundBase*> SpeechLines, FName Speaker0 = "Vincent", FName Speaker0Camera = "ConversationCamera1", FName Speaker1 = "None", FName Speaker1Camera = "ConversationCamera1", FString Script = "", bool ChangeCameras = true, bool ShowSpeakers = false, float DelayBetweenLines = 2, float DelayBetweenSpeakers = 1, float WaitBefore = 0, bool Active = true);

		UFUNCTION(BlueprintCallable, Category = "MUKA")
		static void ConversationLine(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName SpeakerOverride, USoundBase* SpeechLine, FName Speaker0 = "Vincent", FName Speaker0Camera = "ConversationCamera1", FString Script = "", bool ChangeCameras = true, bool ShowSpeakers = false, float DelayBetweenLines = 2, float DelayBetweenSpeakers = 1, float WaitBefore = 0, bool Active = true);
};
