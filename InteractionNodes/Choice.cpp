// Fill out your copyright notice in the Description page of Project Settings.


#include "Choice.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Branch.h"
#include "Components/ProgressBar.h"
#include "../HUD/SFSHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../Inventory.h"

#define VINCENT "Vincent"

void UChoice::InitNode() {
    Timeout = 0;
    Elapsed = 0;
    CurrentState = &UChoice::ChangeCamera;
    Inventory = GetInventory();
}

void UChoice::ChangeCamera() {
    AActor* Vincent = GetActorByTag(Speaker);
     if (Vincent) {
        TArray <AActor*> ChildActors;
        Vincent->GetAttachedActors(ChildActors);
        // UE_LOG(LogTemp, Warning, TEXT("UChoice: Child Actors Array Length %d"), ChildActors.Num());
        for (auto &&ChildActor : ChildActors) {
            // UE_LOG(LogTemp, Warning, TEXT("UChoice: Tag 0 %s"), *ChildActor->Tags[0].ToString());
            if (ChildActor->ActorHasTag(Camera)) {
                // UE_LOG(LogTemp, Warning, TEXT("UChoice: Found Camera %s"), *ChildActor->GetName());
                GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(ChildActor);
                break;
            }
        }
     }
     CurrentState = &UChoice::StartChoices;
}

void UChoice::StartChoices() {
    ASFSHUD* HUD = Cast<ASFSHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (HUD) {
		ChoiceWidget = HUD->GetConversationChoiceWidget();
        ChoicesVerticalBox = Cast<UVerticalBox>(ChoiceWidget->GetWidgetFromName(TEXT("ContextChoice")));
            
        if (ChoicesVerticalBox) {
            for (int i = 0; i < 5; i++) {
            UButton *Button = Cast<UButton>(ChoicesVerticalBox->GetChildAt(i));
            if (Button) {
                Button->Visibility = ESlateVisibility::Hidden;
                UTextBlock* ButtonText = Cast<UTextBlock>(Button->GetChildAt(0));
                if (ButtonText) ButtonText->SetText(FText::FromString(TEXT("")));
            }
            }
        }

        uint8 ButtonsShowing = 0;
        if (ChoicesVerticalBox) {
            for (auto &&Node : NodesMap) {
                UBranch* Branch = Cast<UBranch>(Node.Value);
                if (Branch->BranchName == "Default") continue;
                if (Inventory) {
                    bool InventoryItemForBranch = Inventory->CheckInventory(Branch->WhenInventoryItems);
                    if (!InventoryItemForBranch) continue;

                    if (Branch->Timeout > 0) {
                        UE_LOG(LogTemp, Warning, TEXT("Choice: Found Branch with Timeout %f"), Branch->Timeout);
                        Timeout = Branch->Timeout;
                        TimeoutNode = Branch;
                        continue;
                    }
                UButton* Button = Cast<UButton>(ChoicesVerticalBox->GetChildAt(ButtonsShowing));
                if (Button) {
                    UTextBlock* ButtonText = Cast<UTextBlock>(Button->GetChildAt(0));
                    if (ButtonText) ButtonText->SetText(FText::FromString(Node.Key));
                    if (Branch) {
                        Branch->ConversationChoice = this;
                        Button->OnReleased.AddDynamic(Branch, &UBranch::UpdateConversationChoiceNextNode);
                    }
                    Button->Visibility = ESlateVisibility::Visible;
                    ButtonsShowing++;
                }
                }
            }
        }
        for (int i = ButtonsShowing; i < 5; i++) {
		    if (ChoicesVerticalBox) {
				UButton *Button = Cast<UButton>(ChoicesVerticalBox->GetChildAt(i));
				if (Button) Button->Visibility = ESlateVisibility::Hidden;
			}
        }

        if (ButtonsShowing > 0) {
            if (ChoiceWidget) ChoiceWidget->SetVisibility(ESlateVisibility::Visible);
            ChoiceTimeoutBar = Cast<UProgressBar>(ChoiceWidget->GetWidgetFromName(TEXT("TimeoutBar")));
            if (Timeout > 0) {
                if (ChoiceTimeoutBar) ChoiceTimeoutBar->SetVisibility(ESlateVisibility::Visible);
                UE_LOG(LogTemp, Warning, TEXT("Choice: Set State to Wait with Timeout %f"), Timeout);
                CurrentState = &UChoice::WaitForChoiceWithTimeout;
            } else {
                 if (ChoiceTimeoutBar) ChoiceTimeoutBar->SetVisibility(ESlateVisibility::Collapsed);
                 CurrentState = &UChoice::WaitForChoice;
            }
        } else {
            if (ChoiceWidget) ChoiceWidget->SetVisibility(ESlateVisibility::Collapsed);
            NextNode = NodesMap.Contains("Default") ? NodesMap["Default"] : nullptr;
            CurrentState = &UChoice::End;
        }
    }
}

void UChoice::WaitForChoiceWithTimeout() {
    Elapsed += GetWorld()->GetDeltaSeconds();
    // UE_LOG(LogTemp, Warning, TEXT("Choice:Elapsed %f"), Elapsed);
    // float Progress = 1 - (Elapsed/Timeout);
    if (ChoiceTimeoutBar) {
        ChoiceTimeoutBar->SetPercent(1.0 - (Elapsed/Timeout));
        if (ChoiceTimeoutBar->Percent < 0) {
            SetNodeAndFinish(TimeoutNode);
        }
    }
}

void UChoice::SetNodeAndFinish(UBranch* Node) {
    if (ChoiceWidget) ChoiceWidget->SetVisibility(ESlateVisibility::Collapsed);
    // UWidgetBlueprintLibrary::SetInputMode_GameAndUI(GetWorld()->GetFirstPlayerController());
    if (ChoicesVerticalBox) {
        for (int i = 0; i < 5; i++) {
			UButton *Button = Cast<UButton>(ChoicesVerticalBox->GetChildAt(i));
			if (Button) Button->OnReleased.Clear();
		}
    }
    if (Node->RunOnce) NodesMap.Remove(Node->BranchName);
    NextNode = Node;
    CurrentState = &UChoice::End;
}

// void UChoice::Bye() {
//     //  NextNode = NULL;
//     //  HideChoicesBox();
//      CurrentState = &UChoice::End;
// }

// void UChoice::RemoveOptionAtIndex(int32 index) {
//     Choices.RemoveAt(index);
// }

// void UChoice::HideChoicesBox() {
//     ContextChoiceWidget->RemoveFromParent();
// }

void UChoice::WaitForChoice() {}

bool UChoice::Exec() {
	(this->*CurrentState)();
	return CurrentState == &UChoice::End;
}