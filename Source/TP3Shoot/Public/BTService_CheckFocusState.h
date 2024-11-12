// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckFocusState.generated.h"

/**
 * 
 */
UCLASS()
class TP3SHOOT_API UBTService_CheckFocusState : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckFocusState();

protected:
	/** Appelé lorsque le service devient pertinent (par exemple, quand l'IA entre en mode visée) */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** Appelé lorsque le service cesse d'être pertinent (par exemple, quand l'IA quitte le mode visée) */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
