// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire_BlackboardBase.h"
#include "AIController.h"
#include "AIShootCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TP3Shoot/TP3ShootCharacter.h"

UBTTask_Fire_BlackboardBase::UBTTask_Fire_BlackboardBase()
{
	
	NodeName = TEXT("Fire Gun");
}

EBTNodeResult::Type UBTTask_Fire_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AIController
	AAIController* AIController = OwnerComp.GetAIOwner();
	AAIShootCharacter* AIActor = Cast<AAIShootCharacter>(AIController->GetPawn());
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	// Get TargetPosition
	if(AIActor)
	{
		AIActor->Fire(Blackboard->GetValueAsVector("TargetLocation"));
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIActor is nullptr in UBTTask_Fire_BlackboardBase::ExecuteTask"));
	}
	
	
	// // Obtain the Navigation System and find a random location
	// const UNavigationSystemV1* NavSystem {UNavigationSystemV1::GetCurrent(GetWorld())};
	// if(IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(TargetPosition, 5.0, Location))
	// {
	// 	AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	// }

	// Signal the BehaviorTreeComponent that the task finished with a Success!
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_Fire_BlackboardBase::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
