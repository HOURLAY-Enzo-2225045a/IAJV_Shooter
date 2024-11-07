// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseAIController.h"

#include "AIShootCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TP3Shoot/TP3ShootCharacter.h"


// Sets default values
AChaseAIController::AChaseAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    SetPerceptionComponent(*PerceptionComponent);
	
    // Create the sight config
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AChaseAIController::OnTargetPerceptionUpdated);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AChaseAIController::OnTargetPerceptionForgotten);
}

// Called when the game starts or when spawned
void AChaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(BehaviorTree.Get()))
	{
		RunBehaviorTree(BehaviorTree.Get());
		BehaviorTreeComponent->StartTree(*BehaviorTree.Get());
	}
	
}

void AChaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(IsValid(Blackboard.Get()) && IsValid(BehaviorTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get());
	}
}

// Called every frame
void AChaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus const Stimulus)
{
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,"EventFired");
	// Check actor is a TP3ShootCharacter
	ATP3ShootCharacter* ShootCharacter = Cast<ATP3ShootCharacter>(Actor);
	AAIShootCharacter* AICharacter = Cast<AAIShootCharacter>(Actor);
	
	ACharacter* ChosenCharacter;
	//UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), ( Cast<AAIShootCharacter>(GetPawn())->GetIsEnemy() ? TEXT("true") : TEXT("false") ));

	if((ShootCharacter) && Cast<AAIShootCharacter>(GetPawn())->GetIsEnemy()) // si le personnage est le joueur et sa propre équipe est l'équipe ennemie.
	{
		ChosenCharacter = ShootCharacter;
	
		}else if(AICharacter && AICharacter->GetIsEnemy() != Cast<AAIShootCharacter>(GetPawn())->GetIsEnemy())//Si le personnage est une IA et elle est dans la meme équipe que la sienne.
	 {
			UE_LOG(LogTemp,Display,TEXT("ChosenCharacter "));
		ChosenCharacter = AICharacter;
	 }else
	 {
		return;
	 }
	//if(!ShootCharacter) return;
	// Get the team id of the AI character
	//int TeamId = Cast<ATP3ShootCharacter>(GetPawn())->TeamId;
	//if (!ShootCharacter || ShootCharacter->TeamId == TeamId) return;
	// check if stimulus is sight
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		float Age = Stimulus.GetAge();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Stimulus Age: %f"), Age));
		// if stimulus is sight, check if stimulus is sensed
		if (Stimulus.WasSuccessfullySensed())
		{
			// Set can see player to true
			BlackboardComponent->SetValueAsBool("CanSeePlayer", true);
			// Set target actor
			BlackboardComponent->SetValueAsObject("Target", ChosenCharacter);
			//BlackboardComponent->SetValueAsVector("LastKnownLocation", ChosenCharacter->GetActorLocation());
			GEngine->AddOnScreenDebugMessage(4,5.0f,FColor::Orange,"Enter AI Sight");
		}
		/*else 
		{
			if (Stimulus.GetAge() < 6.0f)
			{
				BlackboardComponent->SetValueAsVector("LastKnownPosition", Stimulus.StimulusLocation);
			}
			else 
			{
				BlackboardComponent->ClearValue("LastKnownPosition");
				BlackboardComponent->SetValueAsBool("CanSeePlayer", false);
			}
			GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Orange, "Lost Sight, Keeping Last Known Position");
		}*/
	}
}

void AChaseAIController::OnTargetPerceptionForgotten(AActor* Actor)
{
	BlackboardComponent->SetValueAsBool("CanSeePlayer", false);
	BlackboardComponent->SetValueAsObject("Target", nullptr);
	GEngine->AddOnScreenDebugMessage(4,5.0f,FColor::Orange,"Max Age Reset");
}

