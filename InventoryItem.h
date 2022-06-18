// // Fill out your copyright notice in the Description page of Project Settings.

// #pragma once

// // #include "CoreMinimal.h"
// // #include "UObject/NoExportTypes.h"
// #include "InventoryItem.generated.h"


// UENUM(BlueprintType)
// enum class EInventoryItemA : uint8 {
//   ENone                         UMETA(DisplayName = "None"),
//   E00_1_Coin                    UMETA(DisplayName = "00_1_Coin"),
//   E00_1_InfoNoteIsMissing       UMETA(DisplayName = "00_1_Note Is Missing"),
//   E00_1_InfoAboutMissingGirl    UMETA(DisplayName = "00_1_Info Missing Girl"),
//   E00_1_InfoAboutRCHInTown      UMETA(DisplayName = "00_1_Info RCH In Town"),
//   E00_1_InfoHostileGuard        UMETA(DisplayName = "00_1_Info Hostile Guard")
// };

// UENUM(BlueprintType)
// enum class EInventoryItemB : uint8 {
//   ENone                         UMETA(DisplayName = "None"),
// };

// USTRUCT(BlueprintType)
// struct FInventoryItem {

//    GENERATED_BODY()

//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Item")
//    EInventoryItemA SlotA;
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory Item")
//    EInventoryItemB SlotB;

// };