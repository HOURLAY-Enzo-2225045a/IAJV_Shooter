// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckFocusState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AIShootCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_CheckFocusState::UBTService_CheckFocusState()
{
	NodeName = TEXT("Check Focus State");
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;
}

void UBTService_CheckFocusState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	AAIShootCharacter* AIActor = Cast<AAIShootCharacter>(AIController->GetPawn());

	if (AIActor)
	{
		// Désactive l'orientation par mouvement et active l'orientation du contrôleur en mode de visée
		AIActor->GetCharacterMovement()->bOrientRotationToMovement = false;
		AIActor->bUseControllerRotationYaw = true;
	}
}

void UBTService_CheckFocusState::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	AAIShootCharacter* AIActor = Cast<AAIShootCharacter>(AIController->GetPawn());

	if (AIActor)
	{
		// Réactive l'orientation par mouvement et désactive l'orientation du contrôleur quand on quitte la visée
		AIActor->GetCharacterMovement()->bOrientRotationToMovement = true;
		AIActor->bUseControllerRotationYaw = false;
	}
}