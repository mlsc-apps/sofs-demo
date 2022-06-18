// // Fill out your copyright notice in the Description page of Project Settings.


// #include "ReplaceInteractions.h"
// #include "../Interaction.h"
// #include "../Interactable.h"

// void UReplaceInteractions::InitNode() {
// 	CurrentState = &UReplaceInteractions::StartReplacing;
// }

// void UReplaceInteractions::StartReplacing() {
// 	if (!ExternalInteractionOwner) {
// 		ExternalInteractionOwner = ParentInteraction->ParentInteractable->GetOwner();
// 	}
// 	AInteraction* ReplaceInteraction = Cast<AInteraction>(GetInteractionByTag(ExternalInteractionOwner, TagToReplace));
//     AInteraction* ReplaceInteractionWith = Cast<AInteraction>(GetInteractionByTag(ExternalInteractionOwner, TagToReplaceWith));
// 	if (ReplaceInteraction && ReplaceInteractionWith) {
// 			ReplaceInteraction->Showable = false;
//             ReplaceInteractionWith->Showable = true;
// 		} else {
// 			UE_LOG(LogTemp, Warning, TEXT("UReplaceInteractions: Interactions to replace not found!"));
// 		}
// 	CurrentState = &UReplaceInteractions::End;
// }

// bool UReplaceInteractions::Exec() {
// 	(this->*CurrentState)();
// 	return CurrentState == &UReplaceInteractions::End;
// } 