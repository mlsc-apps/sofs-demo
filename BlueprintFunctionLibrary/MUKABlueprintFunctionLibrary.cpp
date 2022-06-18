// Fill out your copyright notice in the Description page of Project Settings.

#include "MUKABlueprintFunctionLibrary.h"
#include "../Interaction.h"
#include "../InteractionNodes/InteractionNode.h"
#include "../InteractionNodes/Monologue.h"
#include "../InteractionNodes/PlayAnimation.h"
#include "../InteractionNodes/PlaySequence.h"
#include "../InteractionNodes/ItemGateway.h"
#include "../InteractionNodes/QuestItemGateway.h"
#include "../InteractionNodes/UpdateQuest.h"
#include "../InteractionNodes/AddInventory.h"
#include "../InteractionNodes/ActivateInteractable.h"
#include "../InteractionNodes/Use.h"
#include "../InteractionNodes/Branch.h"
#include "../InteractionNodes/ApplyImpulse.h"
#include "../InteractionNodes/BlendToCamera.h"
#include "../InteractionNodes/Destroy.h"
#include "../InteractionNodes/PlaceActor.h"
#include "../InteractionNodes/PlaySound.h"
#include "../InteractionNodes/RemoveInventory.h"
#include "../InteractionNodes/IndexGateway.h"
#include "../InteractionNodes/CompositeItemGateway.h"
#include "../InteractionNodes/JumpToNode.h"
#include "../InteractionNodes/Choice.h"
#include "../InteractionNodes/Conversation.h"
#include "../InteractionNodes/Fork.h"
#include "Kismet/GameplayStatics.h"

void UMUKABlueprintFunctionLibrary::FinalizeNode(UInteraction* Interaction, UInteractionNode* ThisNode, UInteractionNode* PreviousNode, bool BlockInput, float WaitBefore, bool Active) {
	if (PreviousNode != nullptr) {
		PreviousNode->NextNode = ThisNode;
	}
	ThisNode->ParentInteraction = Interaction;
	if (ThisNode->NName.Len() == 0) {
		ThisNode->NName = ThisNode->GetName();
	}
	
	ThisNode->Active = Active;
	ThisNode->WaitBefore = WaitBefore;
	ThisNode->BlockInput = BlockInput;
	ThisNode->ParentInteraction->InteractionNodes.Add(ThisNode);
}

// Custom K2 Nodes

UInteraction* UMUKABlueprintFunctionLibrary::AddInteraction(UInteractable* Interactable, FString Action, FString Name, EFinalizeWith FinalizeWith, FString WhenInventoryItems, bool RunOnPlay, bool Auto, bool Showable) {
	// check(Interactable);
	if (Interactable) {
		UInteraction* i = NewObject<UInteraction>(Interactable, UInteraction::StaticClass());
		i->Showable = Showable;
		i->Action = Action;
		i->ParentInteractable = Interactable;
		i->FinalizeWith = FinalizeWith;
		i->Conditional = false;
		i->RunOnPlay = RunOnPlay;
		i->WhenInventoryItems = WhenInventoryItems;
		i->Auto = Auto;
		if (Name.Len() > 0) i->Name = Name;
		Interactable->Interactions.Add(Name, i);
		return i;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error adding Interaction node %s: Interactable is null"), *Name);
	}

	return nullptr;
}

UInteractionNode* UMUKABlueprintFunctionLibrary::Monologue(UInteraction* Interaction, UInteractionNode* PreviousNode, FString Lines, float Delay, FName Speaker, bool BlockInput, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UMonologue* m = NewObject<UMonologue>(Outer, UMonologue::StaticClass());
		m->Delay = Delay;
		m->SayText = Lines;
		m->SpeakerTag = Speaker;
		FinalizeNode(Outer, m, PreviousNode, BlockInput, WaitBefore, Active);
		return m;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating Monologue node %s: Interaction is null"), *Lines );
	}

	return nullptr;
	
}

UInteractionNode* UMUKABlueprintFunctionLibrary::AddToInventory(UInteraction* Interaction, UInteractionNode* PreviousNode, FMUKAInventoryItemSpec InventoryItem, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UAddInventory* ai = NewObject<UAddInventory>(Outer, UAddInventory::StaticClass());
		ai->ItemSpecification = InventoryItem;
		FinalizeNode(Outer, ai, PreviousNode, false, WaitBefore, Active);
		return ai;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating AddInventory node: Interaction is null"));
	}

	return nullptr;
}



// Standard K2Nodes

void UMUKABlueprintFunctionLibrary::PlayAnimation(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString ActorTag, UAnimMontage* Montage, UAnimSequence* Animation, bool Loop, bool WaitUntilEnded, bool BlockInput, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UPlayAnimation* pa = NewObject<UPlayAnimation>(Outer, UPlayAnimation::StaticClass());
		pa->Animation = Animation;
		pa->Montage   = Montage;
		pa->Loop = Loop;
		pa->ActorTag = ActorTag;
		pa->WaitUntilEnded = WaitUntilEnded;
		ThisNode = pa;
		FinalizeNode(Outer, pa, PreviousNode, BlockInput, WaitBefore, Active);
		// return pa;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating PlayAnimation node: Interaction is null"));
	}

	// return nullptr;
}

void UMUKABlueprintFunctionLibrary::PlaySequence(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, ULevelSequence* Sequence, bool WaitUntilEnded, bool BlockInput, float WaitBefore, bool Active) {
   	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UPlaySequence* ps = NewObject<UPlaySequence>(Outer, UPlaySequence::StaticClass());
		ps->Sequence = Sequence;
		ps->WaitUntilEnded = WaitUntilEnded;
		ThisNode = ps;
		FinalizeNode(Outer, ps, PreviousNode, BlockInput, WaitBefore, Active);
		// return ps;
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating PlaySequence node: Interaction is null"));
	}

	// return nullptr;
}

void UMUKABlueprintFunctionLibrary::ItemGateway(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString WhenInventoryItems, bool BlockInput, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UItemGateway* ig = NewObject<UItemGateway>(Outer, UItemGateway::StaticClass());
		// ig->Interval = Interval;
		ig->WhenInventoryItems = WhenInventoryItems;
		ThisNode = ig;
		FinalizeNode(Outer, ig, PreviousNode, BlockInput, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating ItemGateway node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::QuestItemGateway(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString WhenInventoryItems, FString QuestStepDescription, bool BlockInput, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UUpdateQuest* UpdateQuestNode = NewObject<UUpdateQuest>(Outer, UUpdateQuest::StaticClass());
		UpdateQuestNode->Active = Active;
    	UpdateQuestNode->WaitBefore = WaitBefore; //WaitBefore;
		UpdateQuestNode->BlockInput = BlockInput; //BlockInput;
		UpdateQuestNode->ParentInteraction = Outer;
		UpdateQuestNode->QuestStepText = QuestStepDescription;
		UpdateQuestNode->NName = UpdateQuestNode->GetName();
		
		UQuestItemGateway* qig = NewObject<UQuestItemGateway>(Outer, UQuestItemGateway::StaticClass());
		qig->WhenInventoryItems = WhenInventoryItems;
		qig->QuestStepText = QuestStepDescription;
		qig->UpdateQuestNode = UpdateQuestNode;
		ThisNode = qig;
		FinalizeNode(Outer, qig, PreviousNode, BlockInput, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating QuestItemGateway node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::UpdateQuest(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, FString QuestStepDescription, bool Mute, bool BlockInput, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UUpdateQuest* uq = NewObject<UUpdateQuest>(Outer, UUpdateQuest::StaticClass());
		// uq->WhenInventoryItems = WhenInventoryItems;
		uq->QuestStepText = QuestStepDescription;
		uq->Mute = Mute;
		ThisNode = uq;
		FinalizeNode(Outer, uq, PreviousNode, BlockInput, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating UpdateQuest node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::FinishQuest(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode, bool Mute, bool BlockInput, float WaitBefore, bool Active) {
	UpdateQuest(Interaction, PreviousNode, ThisNode, TEXT(""), Mute, BlockInput, WaitBefore, Active);
}

void UMUKABlueprintFunctionLibrary::CompositeItemGateway(UInteraction* Interaction, UInteractionNode* PreviousNode, UInteractionNode*& ThisNode) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UCompositeItemGateway* qig = NewObject<UCompositeItemGateway>(Outer, UCompositeItemGateway::StaticClass());
		// qig->WhenInventoryItems = WhenInventoryItems;
		// qig->TransientQuestSteps = TransientQuestSteps;
		// qig->QuestStepText = QuestStepDescription;
		ThisNode = qig;
		FinalizeNode(Outer, qig, PreviousNode, false, 0, true);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating CompositeItemGateway node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::ActivateInteractable(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName ActorTag, bool Activate, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UActivateInteractable* ai = NewObject<UActivateInteractable>(Outer, UActivateInteractable::StaticClass());
		ai->ActorTag = ActorTag;
		ai->Activate = Activate;
		ThisNode = ai;
		FinalizeNode(Outer, ai, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating ActivateInteractable node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::Use(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UUse* use = NewObject<UUse>(Outer, UUse::StaticClass());
		// ai->ActorTag = ActorTag;
		// ai->Activate = Activate;
		ThisNode = use;
		FinalizeNode(Outer, use, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating Use node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::Branch(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString Option, FString WhenInventoryItems, bool RunOnce, float Timeout) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UBranch* br = NewObject<UBranch>(Outer, UBranch::StaticClass());
		br->BranchName = Option;
		br->Timeout = Timeout;
		br->WhenInventoryItems = WhenInventoryItems;
		br->RunOnce = RunOnce;
		UUse* UseNode = Cast<UUse>(PreviousNode);
		if (UseNode) {
			UseNode->NodesMap.Add(br->BranchName, br);
		}
		UCompositeItemGateway* CompositeItemGatewayNode = Cast<UCompositeItemGateway>(PreviousNode);
		if (CompositeItemGatewayNode) {
			CompositeItemGatewayNode->NodesMap.Add(br->BranchName, br);
		}
		UChoice* ChoiceNode = Cast<UChoice>(PreviousNode);
		if (ChoiceNode) {
			ChoiceNode->NodesMap.Add(br->BranchName, br);
		}
		UFork* ForkNode = Cast<UFork>(PreviousNode);
		if (ForkNode) {
			ForkNode->NodesMap.Add(br->BranchName, br);
		}
		ThisNode = br;
		FinalizeNode(Outer, br, PreviousNode, false, 0, true);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating Branch node: Interaction is null"));
	}
}

AInventory* UMUKABlueprintFunctionLibrary::GetInventory(APlayerController* PlayerController) {
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(PlayerController->GetWorld(), TEXT("Inventory"), FoundActors);
    if (FoundActors.Num() > 0) {
		return Cast<AInventory>(FoundActors[0]);
	}
	return nullptr;
}

void UMUKABlueprintFunctionLibrary::ApplyImpulse(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName ActorTag, FVector Force, FVector ForceLocation, USoundBase* Sound, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UApplyImpulse* ai = NewObject<UApplyImpulse>(Outer, UApplyImpulse::StaticClass());
		ai->ActorTag = ActorTag;
		ai->Force = Force;
		ai->ForceLocation = ForceLocation;
		ai->Sound = Sound;
		ThisNode = ai;
		FinalizeNode(Outer, ai, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating ApplyImpulse node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::ChangeCamera(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName Actor, float BlendTime, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UBlendToCamera* btoc = NewObject<UBlendToCamera>(Outer, UBlendToCamera::StaticClass());
		btoc->BlendTime = BlendTime;
		btoc->ActorTag = Actor;
		ThisNode = btoc;
		FinalizeNode(Outer, btoc, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating ChangeCamera node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::Destroy(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName Actors, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UDestroy* ds = NewObject<UDestroy>(Outer, UDestroy::StaticClass());
	 	ds->Tag = Actors;
		ThisNode = ds;
		FinalizeNode(Outer, ds, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating Destroy node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::PlaceActor(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName ActorTag, FName TargetTag, FVector Location, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UPlaceActor* pa = NewObject<UPlaceActor>(Outer, UPlaceActor::StaticClass());
	 	pa->Location = Location;
		pa->ActorTag = ActorTag;
		pa->TargetTag = TargetTag;
		ThisNode = pa;
		FinalizeNode(Outer, pa, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating PlaceActor node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::PlaySound(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, USoundBase* Sound, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UPlaySound* ps = NewObject<UPlaySound>(Outer, UPlaySound::StaticClass());
	 	ps->Sound = Sound;
		ThisNode = ps;
		FinalizeNode(Outer, ps, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating PlaySound node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::RemoveFromInventory(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString InventoryItemName, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		URemoveInventory* ri = NewObject<URemoveInventory>(Outer, URemoveInventory::StaticClass());
	 	ri->Name = InventoryItemName;
		ThisNode = ri;
		FinalizeNode(Outer, ri, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating RemoveFromInventory node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::IndexGateway(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, int MaximumCount, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UIndexGateway* ig = NewObject<UIndexGateway>(Outer, UIndexGateway::StaticClass());
	 	ig->MaximumCount = MaximumCount;
		ThisNode = ig;
		FinalizeNode(Outer, ig, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating IndexGateway node: Interaction is null"));
	}
}

void UMUKABlueprintFunctionLibrary::JumpToNode(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString NextNodeName, UInteractionNode* NextNode, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UJumpToNode* jtn = NewObject<UJumpToNode>(Outer, UJumpToNode::StaticClass());
		jtn->JumpNode = NextNode;
		jtn->JumpNodeName = NextNodeName;
		ThisNode = jtn;
		FinalizeNode(Outer, jtn, PreviousNode, false, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating JumpToNode node: Outer is null"));
	}
}

void UMUKABlueprintFunctionLibrary::ConversationChoice(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString NodeName, FName Speaker, FName Camera, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UChoice* cn = NewObject<UChoice>(Outer, UChoice::StaticClass());
	 	cn->Camera = Camera;
		cn->Speaker = Speaker;
		cn->NName = NodeName;
		ThisNode = cn;
		FinalizeNode(Outer, cn, PreviousNode, true, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating ConversationChoice node: Outer is null"));
	}
}

void UMUKABlueprintFunctionLibrary::Conversation(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName SpeakerToShow, FName Speaker0, FName Speaker0Camera, FName Speaker1, FName Speaker1Camera, FString Script, bool ChangeCameras, bool ShowSpeakers, float DelayBetweenLines, float DelayBetweenSpeakers, float WaitBefore, bool Active) {
	TArray<USoundBase*> SpeechEmpty;
	ConversationWithSpeech(PreviousNode, Interaction, ThisNode, SpeakerToShow, SpeechEmpty, Speaker0, Speaker0Camera, Speaker1, Speaker1Camera, Script, ChangeCameras, ShowSpeakers, DelayBetweenLines, DelayBetweenSpeakers, WaitBefore, Active);
}

void UMUKABlueprintFunctionLibrary::ConversationWithSpeech(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName SpeakerToShow, TArray<USoundBase*> SpeechLines, FName Speaker0, FName Speaker0Camera, FName Speaker1, FName Speaker1Camera, FString Script, bool ChangeCameras, bool ShowSpeakers, float DelayBetweenLines, float DelayBetweenSpeakers, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UConversation* cn = NewObject<UConversation>(Outer, UConversation::StaticClass());
	 	cn->Speaker0 = Speaker0;
		cn->Speaker1 = Speaker1;
		cn->Speaker0Camera = Speaker0Camera;
		cn->Speaker1Camera = Speaker1Camera;
		cn->Script = Script;
		cn->DelayBetweenLines = DelayBetweenLines;
		cn->DelayBetweenSpeakers = DelayBetweenSpeakers;
		cn->ChangeCameras = ChangeCameras;
		cn->ShowSpeakers = ShowSpeakers;
		cn->Speech = SpeechLines;
		cn->SpeakerToShow = SpeakerToShow;
		ThisNode = cn;
		FinalizeNode(Outer, cn, PreviousNode, true, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating Conversation node: Outer is null"));
	}
}

void UMUKABlueprintFunctionLibrary::Fork(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FString NodeName) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		UFork* fo = NewObject<UFork>(Outer, UFork::StaticClass());
	 	// fo->WhenInventoryItems = WhenInventoryItems;
		 fo->NName = NodeName;
		ThisNode = fo;
		FinalizeNode(Outer, fo, PreviousNode, false, 0, true);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating Fork node: Outer is null"));
	}
}

void UMUKABlueprintFunctionLibrary::ConversationLine(UInteractionNode* PreviousNode, UInteraction* Interaction, UInteractionNode*& ThisNode, FName SpeakerOverride, USoundBase* SpeechLine, FName Speaker0, FName Speaker0Camera, FString Script, bool ChangeCameras, bool ShowSpeakers, float DelayBetweenLines, float DelayBetweenSpeakers, float WaitBefore, bool Active) {
	UInteraction* Outer = Interaction ? Interaction : PreviousNode ? PreviousNode->ParentInteraction : nullptr;
	if (Outer) {
		TArray<USoundBase*> Speech;
		if (SpeechLine) Speech.Add(SpeechLine);
		UConversation* cn = NewObject<UConversation>(Outer, UConversation::StaticClass());
	 	cn->Speaker0 = Speaker0;
		// cn->Speaker1 = Speaker1;
		cn->Speaker0Camera = Speaker0Camera;
		// cn->Speaker1Camera = Speaker1Camera;
		cn->Script = Script;
		cn->DelayBetweenLines = DelayBetweenLines;
		cn->DelayBetweenSpeakers = DelayBetweenSpeakers;
		cn->ChangeCameras = ChangeCameras;
		cn->ShowSpeakers = ShowSpeakers;
		cn->Speech = Speech;
		cn->SpeakerToShow = SpeakerOverride;
		ThisNode = cn;
		FinalizeNode(Outer, cn, PreviousNode, true, WaitBefore, Active);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Error creating ConversationLine node: Outer is null"));
	}
}