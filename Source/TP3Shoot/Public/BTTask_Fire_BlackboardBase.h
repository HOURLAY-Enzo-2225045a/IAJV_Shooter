// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Fire_BlackboardBase.generated.h"

/**
 * 
 */
UCLASS()
class TP3SHOOT_API UBTTask_Fire_BlackboardBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_Fire_BlackboardBase();
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;
};
