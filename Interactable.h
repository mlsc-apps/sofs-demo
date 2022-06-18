// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "Styling/SlateColor.h"
#include "InventoryItem.h"
#include "Engine/DataTable.h"
#include "Misc/EnumRange.h"
#include "Interactable.generated.h"

class UInteraction;
class AInventory;
class USoundCue;
class UTexture2D;

// namespace FinalizeInteractionWith {
// 	FString Reset   = "Reset";
//     FString Destroy = "Destroy";
// };

UENUM(BlueprintType)
enum class EFinalizeWith : uint8 {
	Reset     UMETA(DisplayName = "Reset"),
	Destroy   UMETA(DisplayName = "Destroy")
};

ENUM_RANGE_BY_COUNT(EFinalizeWith, 2);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MUKA_API UInteractable : public UActorComponent
{
	GENERATED_BODY()

private:
	URichTextBlock* Label;
    UVerticalBox* Menu;
	FVector ActorCenter;
	
public:
	// Sets default values for this component's properties
	UInteractable();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MUKA")
	bool Active = true;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	AActor *ContextLabel;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	AActor *ContextMenu;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	AInventory *Inventory;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	TMap<FString, UInteraction*> Interactions;
	// TArray<UInteraction*> Interactions;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	UInteraction* CurrentInteraction;

	UPROPERTY(EditAnywhere, Category = "MUKA")
	bool HasLabel = true;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	bool Speaker;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = Speaker), Category = "MUKA")
	FSlateColor TextCloudColor;
	// UPROPERTY(EditAnywhere, Category = "MUKA")
	// bool IsAudible;
	
	// UPROPERTY(EditAnywhere, Category = "MUKA")
	// bool CreatesChoiceContext;
	
	// UPROPERTY(EditAnywhere, meta = (EditCondition = CreatesChoiceContext), Category = "MUKA")
	// TSubclassOf<class UUserWidget> ContextChoice;
	
	UPROPERTY(EditAnywhere, Category = "MUKA")
	bool InventoryItem;

    UPROPERTY(EditAnywhere, meta = (EditCondition = InventoryItem), Category = "MUKA")
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, meta = (EditCondition = InventoryItem), Category = "MUKA")
	FString Description;

	UFUNCTION()
	void ShowLabel(UPrimitiveComponent *TouchedComponent);
	UFUNCTION()
	void HideLabel(UPrimitiveComponent *TouchedComponent);
	UFUNCTION()
	void ShowMenu(UPrimitiveComponent* touchedComponent, FKey buttonPressed);
	// UFUNCTION()
	// void PlaySoundOnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// UFUNCTION(BlueprintCallable, Category = "MUKA")
	// UFUNCTION(meta = (BlueprintInternalUseOnly = "true"), BlueprintCallable, Category =  "MUKA")
    // UInteraction* AddInteraction(FString Action, FString Name, EFinalizeWith FinalizeWith, bool TestOnPlay, bool Showable=true);
	UFUNCTION(BlueprintCallable)
    UInteraction* GetInteractionByName(FString Name);

	UFUNCTION(BlueprintCallable)
    void RunInteractionByName(FString Name);

	UFUNCTION(BlueprintCallable)
    void Engage();

	UFUNCTION(BlueprintCallable)
    void Disengage();

    UFUNCTION(BlueprintCallable)
	void ResetInteractable(UInteractable*& Interactable);

	UInteraction* RunInteractionWithHash(FString Name, int index);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// virtual void InitializeComponent() override;
	// OnComponentActivated
};
