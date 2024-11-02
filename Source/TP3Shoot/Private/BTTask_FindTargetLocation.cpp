// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindTargetLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TP3Shoot/TP3ShootCharacter.h"

UBTTask_FindTargetLocation::UBTTask_FindTargetLocation()
{
	NodeName = TEXT("Find Target Location");
}

EBTNodeResult::Type UBTTask_FindTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};

	// Get AIController
	AAIController* AIController = OwnerComp.GetAIOwner();

	// Get TargetPosition
	const ATP3ShootCharacter* Target = Cast<ATP3ShootCharacter>(AIController->GetBlackboardComponent()->GetValueAsObject("Target"));
	const FVector TargetPosition = Target->GetActorLocation();

	// Obtain the Navigation System and find a random location
	const UNavigationSystemV1* NavSystem {UNavigationSystemV1::GetCurrent(GetWorld())};
	if(IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(TargetPosition, 5.0, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}

	// Signal the BehaviorTreeComponent that the task finished with a Success!
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_FindTargetLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
