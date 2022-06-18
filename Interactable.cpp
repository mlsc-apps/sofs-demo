// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/RichTextBlock.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Inventory.h"
#include "Interaction.h"
#include "BlueprintFunctionLibrary/MUKABlueprintFunctionLibrary.h"
#include "Misc/StringBuilder.h"

#define INVENTORY     "Inventory"
#define CONTEXT_MENU  "ContextMenu"
#define CONTEXT_LABEL "ContextLabel"
#define SPACE        " "
#define UNDERSCORE   "_"
#define HASH         "#"
#define MAX_BUTTONS  3
#define MAX_INTERACTION_OVERRIDES 5
#define VINCENT      "Vincent"

// Sets default values for this component's properties
UInteractable::UInteractable() {
	PrimaryComponentTick.bCanEverTick = true;
	if (GetOwner()) {
		UMeshComponent *mesh = nullptr;
		mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		if (!mesh) {
			mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
		}
		if (mesh) {
			mesh->OnBeginCursorOver.AddDynamic(this, &UInteractable::ShowLabel);
			mesh->OnEndCursorOver.AddDynamic(this, &UInteractable::HideLabel);
			mesh->OnClicked.AddDynamic(this, &UInteractable::ShowMenu);
			// if (IsAudible) {
			// 	mesh->OnComponentBeginOverlap.AddDynamic(this, &UInteractable::PlaySoundOnHit);
			// }
		}
	}
}

// Called when the game starts
void UInteractable::BeginPlay() {
	Super::BeginPlay();
	// if (CurrentInteraction) {
		// CurrentInteraction->Run();
	// }
	for (auto InteractionMap : Interactions) {
		UInteraction* Interaction = InteractionMap.Value;
		if (Interaction->RunOnPlay) Interaction->Run();
	}
}

// Called every frame
void UInteractable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	// for (auto CurrentInteraction : CurrentInteractions) {
		// CurrentInteraction->Tick(DeltaTime);
	// }
	
	if (CurrentInteraction) {
		// if (!Vincent) {
		// 	TArray<AActor *> FoundActors;
		// 	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT(VINCENT), FoundActors);
		// 	if (FoundActors.Num() > 0) Vincent = FoundActors[0];
		// }
		// if (Vincent) {
		//  ACharacter* Vincent = GetWorld()->GetFirstPlayerController()->GetCharacter();
		// //  FVector PlayerLoc = ;
        //  FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Vincent->GetActorLocation(), GetOwner()->GetActorLocation());
        //  FRotator NewRot = FMath::RInterpTo(Vincent->GetActorRotation(), LookAtRotation, DeltaTime, 2);
		// // }
		// if (NewRot.IsZero()) {
			CurrentInteraction->Tick(DeltaTime);
		// } else {
			// Vincent->SetActorRotation(NewRot);
		// }
	}
}

void UInteractable::HideLabel(UPrimitiveComponent *TouchedComponent) {
	if (ContextLabel)
		ContextLabel->SetActorHiddenInGame(true);
}

// void UInteractable::PlaySoundOnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
// 	 UE_LOG(LogTemp, Warning, TEXT("Playing Sound now"));
// }

void UInteractable::ShowLabel(UPrimitiveComponent * TouchedComponent) {
	if (!Active) return;
	if (!ContextLabel) {
		TArray<AActor *> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CONTEXT_LABEL, FoundActors);
		if (FoundActors.Num() > 0) {
			ContextLabel = FoundActors[0];
		}
	}
	
	if (!Label && ContextLabel) {
		UWidgetComponent *widgetcomponent = ContextLabel->FindComponentByClass<UWidgetComponent>();
		UUserWidget *widgetObject = widgetcomponent->GetUserWidgetObject();
		if (widgetObject) {
			Label = Cast<URichTextBlock>(widgetObject->GetWidgetFromName(TEXT(CONTEXT_LABEL)));
		}
	}

	if (Label && ContextLabel && HasLabel) {
		FString Name = GetOwner()->GetName();
		if (Name.Contains(SPACE)) {
			FString Right;
			Name.Split(SPACE, &Name, &Right);
		}
		if (Name.Contains(UNDERSCORE)) {
			FString Right;
			Name.Split(UNDERSCORE, &Name, &Right);
		}
		FString DisplayString = FName::NameToDisplayString(Name, false);
		Label->SetText(FText::FromString(DisplayString));
		FVector Origin; FVector BoxEntent;
		GetOwner()->GetActorBounds(true, Origin, BoxEntent);
		// mesh->GetTrans
		Origin.X = UKismetMathLibrary::Round(Origin.X);
		Origin.Y = UKismetMathLibrary::Round(Origin.Y);
		Origin.Z = UKismetMathLibrary::Round(Origin.Z);
		ContextLabel->SetActorLocation(Origin);
		ContextLabel->SetActorHiddenInGame(false);
	}
}

void UInteractable::ShowMenu(UPrimitiveComponent *touchedComponent, FKey buttonPressed) {
	if (!Active) return;
	if (!GetOwner()) return;
	if (!ContextMenu) {
		TArray<AActor *> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT(CONTEXT_MENU), FoundActors);
		if (FoundActors.Num() > 0) {
			ContextMenu = FoundActors[0];
		}
	}
	if (buttonPressed == EKeys::RightMouseButton) {
		int buttonsShowing = 0;
		// bool ItemPresentInInventory = false;
		
		if (!Inventory) {
			TArray<AActor *> FoundInventory;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT(INVENTORY), FoundInventory);
			if (FoundInventory.Num() > 0) Inventory = Cast<AInventory>(FoundInventory[0]);
		}

		TMap<FString, bool> InteractionAlreadyEvaluated;
		
		for (auto& InteractionItem : Interactions) {
			UInteraction *Interaction = Cast<UInteraction>(InteractionItem.Value);
			if (!Interaction) continue;
			if (!Interaction->Showable) continue;

			if (Interaction->Name.Contains(HASH)) {
				FString DefaultName;
				FString Number;
				Interaction->Name.Split(HASH, &DefaultName, &Number);
				int InteractionHashNumber = FCString::Atoi(*Number) + 1;
				DefaultName.AppendChar('#').AppendInt(InteractionHashNumber);
				UE_LOG(LogTemp, Warning, TEXT("\nUInteractable: Hash in interactiom %s Found. Check for %s"), *Interaction->Name, *DefaultName);
				if (Interactions.Contains(DefaultName)) {
					UE_LOG(LogTemp, Warning, TEXT("UInteractable: %s exists. Checking Inventory Query for %s"), *DefaultName, *DefaultName);
					if (Inventory) {
						if (Inventory->CheckInventory(Interactions[DefaultName]->WhenInventoryItems)) {
							InteractionAlreadyEvaluated.Add(DefaultName, true);
							UE_LOG(LogTemp, Warning, TEXT("Success. Current Interaction will be overriden by %s"), *DefaultName);
							continue;
						} else {
							UE_LOG(LogTemp, Warning, TEXT("Failed. %s will be marked as evaluated and failed"), *DefaultName);
							InteractionAlreadyEvaluated.Add(DefaultName, false);
						}				
					}
				}
			}

			if (InteractionAlreadyEvaluated.Contains(Interaction->Name)) {
				if (!InteractionAlreadyEvaluated[Interaction->Name]) {
					UE_LOG(LogTemp, Warning, TEXT("UInteractable: Interaction %s already evaluated to failed. Will be skipped"), *Interaction->Name);
					continue;
				}
				UE_LOG(LogTemp, Warning, TEXT("UInteractable: Interaction %s already evaluated to true"), *Interaction->Name);
			} else {
				if (Inventory) {
					UE_LOG(LogTemp, Warning, TEXT("UInteractable: Check Invnentory Query for %s"), *Interaction->Name);
					bool ItemPresentInInventory = Inventory->CheckInventory(Interaction->WhenInventoryItems);
					if (!ItemPresentInInventory) continue;
				} else {
					UE_LOG(LogTemp, Warning, TEXT("UInteractable: Inventory Actor not found!"));
					return;
				}
			}
			
			if (!Menu) {
				UWidgetComponent *widgetcomponent = ContextMenu->FindComponentByClass<UWidgetComponent>();
				UUserWidget *widgetObject = widgetcomponent->GetUserWidgetObject();
				if (widgetObject) {
					Menu = Cast<UVerticalBox>(widgetObject->GetWidgetFromName(TEXT(CONTEXT_MENU)));
				}
			}
			if (Menu) {
				UButton* Button = Cast<UButton>(Menu->GetChildAt(buttonsShowing));
				if (Button) {
					FString Action = Interaction->Action;
					UTextBlock* ButtonText = Cast<UTextBlock>(Button->GetChildAt(0));
					if (ButtonText) {
						ButtonText->SetText(FText::FromString(Action));
						ButtonText->SetColorAndOpacity(FSlateColor(FLinearColor(255, 255, 255)));
					}
					Button->OnReleased.Clear();
					Button->OnReleased.AddDynamic(Interaction, &UInteraction::Run);
					Button->Visibility = ESlateVisibility::Visible;
					Interaction->Menu = ContextMenu;
					buttonsShowing++;
				}
			}
		}
		
		for (int i = buttonsShowing; i < MAX_BUTTONS; i++) {
			if (Menu) {
				UButton *Button = Cast<UButton>(Menu->GetChildAt(i));
				if (Button) Button->Visibility = ESlateVisibility::Hidden;
			}
		}

		if (ContextMenu && ContextLabel && buttonsShowing > 0) {
				FVector Origin; FVector BoxEntent;
				GetOwner()->GetActorBounds(true, Origin, BoxEntent);
				Origin.X = UKismetMathLibrary::Round(Origin.X);
				Origin.Y = UKismetMathLibrary::Round(Origin.Y);
				Origin.Z = UKismetMathLibrary::Round(Origin.Z);
				ContextMenu->SetActorLocation(Origin);
				ContextMenu->SetActorHiddenInGame(false); // = ESlateVisibility::Visible;
				ContextLabel->SetActorHiddenInGame(true);
				GetWorld()->GetFirstPlayerController()->bEnableMouseOverEvents = false;
			    GetWorld()->GetFirstPlayerController()->bEnableClickEvents = false;
			}
		}
}

UInteraction* UInteractable::GetInteractionByName(FString Name) {
	// UE_LOG(LogTemp, Warning, TEXT("Getting Interaction %s"), *Name);
	if (Interactions.Contains(Name)) {
		return Interactions[Name];
	}
	return NULL;
}

void UInteractable::RunInteractionByName(FString Name) {
	UInteraction* Interaction = GetInteractionByName(Name);
	if (!Interaction) {
		Interaction = RunInteractionWithHash(Name, 0);
	}
	if (Interaction) {
		// FString BaseName;
		// FString Number;
		// Interaction->Name.Split(HASH, &BaseName, &Number);
		// int InteractionHashNumber = FCString::Atoi(*Number) + 1;
		// BaseName.AppendChar('#').AppendInt(InteractionHashNumber);
		// RunInteractionByName(BaseName);
		Interaction->Run();
	}
}

UInteraction* UInteractable::RunInteractionWithHash(FString Name, int index) {
	FString BaseName = Name;
	if (!Inventory) {
		Inventory = UMUKABlueprintFunctionLibrary::GetInventory(GetWorld()->GetFirstPlayerController());
	}
	if (Inventory) {
		Name.AppendChar('#').AppendInt(index);
		UE_LOG(LogTemp, Warning, TEXT("Getting Interaction %s"), *Name);
		UInteraction* Interaction = GetInteractionByName(Name);
		if (Interaction) {
			index++;
			UInteraction* Override = RunInteractionWithHash(BaseName, index);
			if (Override) {
				return Inventory->CheckInventory(Override->WhenInventoryItems) ? Override : nullptr;
			} else {
				return Inventory->CheckInventory(Interaction->WhenInventoryItems) ? Interaction : nullptr;
			}
		}
	}
	return nullptr;
}

void UInteractable::ResetInteractable(UInteractable*& Interactable) {
	Interactions.Empty();
	CurrentInteraction = nullptr; //s.Empty();
	Interactable = this;
}

void UInteractable::Engage() {
	ShowLabel(nullptr);
}

void UInteractable::Disengage() {
	HideLabel(nullptr);
}