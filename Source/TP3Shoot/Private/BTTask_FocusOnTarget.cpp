// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FocusOnTarget.h"
#include "AIController.h"
#include "AIShootCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_FocusOnTarget::UBTTask_FocusOnTarget()
{
    NodeName = TEXT("Focus Target");
    bNotifyTick = true; // Permet d'appeler TickTask
}

EBTNodeResult::Type UBTTask_FocusOnTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get AIController and AI Actor
    AAIController* AIController = OwnerComp.GetAIOwner();
    AAIShootCharacter* AIActor = Cast<AAIShootCharacter>(AIController->GetPawn());
    UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();

    if (AIActor)
    {
        // Désactive l'orientation automatique vers la direction du mouvement
        AIActor->GetCharacterMovement()->bOrientRotationToMovement = false;
        
        // Obtenir la cible du blackboard
        AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("Target"));
        if (TargetActor)
        {
            // SetFocus pour orienter la vue de l'IA vers la cible
            //AIController->SetFocus(TargetActor);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Target is not an AActor in UBTTask_FocusOnTarget::ExecuteTask"));
        }
    }

    return EBTNodeResult::InProgress;
}

void UBTTask_FocusOnTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    AAIShootCharacter* AIActor = Cast<AAIShootCharacter>(AIController->GetPawn());
    UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();

    if (AIActor)
    {
        AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("Target"));
        if (TargetActor)
        {
            // Calculer la rotation en direction de la cible
            FVector DirectionToTarget = TargetActor->GetActorLocation() - AIActor->GetActorLocation();
            FRotator TargetRotation = DirectionToTarget.Rotation();

            // Interpolation de la rotation pour un mouvement fluide
            FRotator NewRotation = FMath::RInterpTo(AIActor->GetActorRotation(), TargetRotation, DeltaSeconds, 6.0f);
            AIActor->SetActorRotation(NewRotation);
        }
    }
}

FString UBTTask_FocusOnTarget::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}