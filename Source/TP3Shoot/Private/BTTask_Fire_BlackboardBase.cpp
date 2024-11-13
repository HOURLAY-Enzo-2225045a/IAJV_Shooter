// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire_BlackboardBase.h"
#include "AIController.h"
#include "AIShootCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "TP3Shoot/TP3ShootCharacter.h"

UBTTask_Fire_BlackboardBase::UBTTask_Fire_BlackboardBase()
{
	
	NodeName = TEXT("Fire Gun");
}

// EBTNodeResult::Type UBTTask_Fire_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
// {
// 	// Get AIController
// 	AAIController* AIController = OwnerComp.GetAIOwner();
// 	AAIShootCharacter* AIActor = Cast<AAIShootCharacter>(AIController->GetPawn());
// 	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
// 	// Get TargetPosition
// 	if(AIActor)
// 	{
// 		AIActor->Fire(Blackboard->GetValueAsVector("TargetLocation"));
// 	}else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("AIActor is nullptr in UBTTask_Fire_BlackboardBase::ExecuteTask"));
// 	}
//
// 	// Signal the BehaviorTreeComponent that the task finished with a Success!
// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);	
// 	return EBTNodeResult::Succeeded;
// }

EBTNodeResult::Type UBTTask_Fire_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AIController and AI Character
	AAIController* AIController = OwnerComp.GetAIOwner();
	AAIShootCharacter* AIActor = Cast<AAIShootCharacter>(AIController->GetPawn());
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();

	if (AIActor)
	{
		// Get Target Actor from the Blackboard
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("Target")); // Assure that "TargetActor" is an Object key in the Blackboard
		
		if (TargetActor)
		{
			// Get central position from the target's CapsuleComponent
			FVector TargetLocation = TargetActor->GetActorLocation(); // Default to actor location
			
			// Try to get the CapsuleComponent or another central component
			UCapsuleComponent* CapsuleComponent = TargetActor->FindComponentByClass<UCapsuleComponent>();
			if (CapsuleComponent)
			{
				TargetLocation = CapsuleComponent->GetComponentLocation();
			}
			
			// Call Fire with the central location
			AIActor->Fire(TargetLocation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TargetActor is nullptr in UBTTask_Fire_BlackboardBase::ExecuteTask"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIActor is nullptr in UBTTask_Fire_BlackboardBase::ExecuteTask"));
	}

	// Signal the BehaviorTreeComponent that the task finished with Success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_Fire_BlackboardBase::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
