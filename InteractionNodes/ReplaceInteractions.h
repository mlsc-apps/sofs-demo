// // Fill out your copyright notice in the Description page of Project Settings.

// #pragma once

// #include "CoreMinimal.h"
// #include "InteractionNode.h"
// #include "ReplaceInteractions.generated.h"

// /**
//  * 
//  */
// UCLASS()
// class MUKA_API UReplaceInteractions : public UInteractionNode
// {
// 	GENERATED_BODY()

// private:
// 	typedef void (UReplaceInteractions::* Action)(void);
// 	Action CurrentState;
	
// public:
// 	UPROPERTY()
// 	FName TagToReplace;
// 	UPROPERTY()
// 	FName TagToReplaceWith;
// 	UPROPERTY()
// 	AActor* ExternalInteractionOwner;
	
// 	void InitNode() override;
// 	bool Exec() override;

// 	void StartReplacing();
	
// };
