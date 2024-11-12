// Fill out your copyright notice in the Description page of Project Settings.


#include "AIShootCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/TimelineComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TP3Shoot/TP3ShootCharacter.h"


// Sets default values
AAIShootCharacter::AAIShootCharacter()
{
 		// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create SK_Gun
	SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	SK_Gun->SetupAttachment(GetMesh());
	// Set parent socket
	SK_Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
	// In the constructor
	// HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	// HealthWidgetComponent->SetupAttachment(RootComponent);
	//
	// // Set the widget class (assuming you have a UUserWidget subclass called UHealthWidget)
	// HealthWidgetComponent->SetWidgetClass(UHealthWidget::StaticClass());

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

bool AAIShootCharacter::GetIsEnemy()
{
	return isEnemy;
}

void AAIShootCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthToMaxRatio = 1.f;
	Health = 100;
	ChaseAIController = Cast<AChaseAIController>(GetController());
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAIShootCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
}

void AAIShootCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AAIShootCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AAIShootCharacter::Aim()
{
	IsAiming = true;
}

void AAIShootCharacter::StopAiming()
{
	IsAiming = false;
}
void AAIShootCharacter::Raycast(FVector StartTrace, FVector EndTrace)
{
	FHitResult* HitResult = new FHitResult();
	FCollisionQueryParams* CQP = new FCollisionQueryParams();
	ECollisionChannel Channel = ECC_Visibility; // Or another channel of your choice
	if(GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace,Channel, *CQP))
	{
		DrawDebugLine(
		GetWorld(),
		StartTrace,
		EndTrace,
		FColor(0, 255, 0),
		false, 2, 0,
		5
		);
		ATP3ShootCharacter* APlayer = Cast<ATP3ShootCharacter>(HitResult->GetActor());
		AAIShootCharacter* AICharacter = Cast<AAIShootCharacter>(HitResult->GetActor());
		
		if(APlayer != NULL)
		{
			APlayer->OnHit(20);
		}
		if(AICharacter !=NULL)
		{
			AICharacter->OnHit(20);
		}
	}
}
void AAIShootCharacter::OnHit(int damage)
{
	Health -= damage;
	HealthToMaxRatio = Health / 100.f;
	//change widget here
	
	if (Health<0.01)
	{
		Die();
	}
	UE_LOG(LogTemp, Warning, TEXT("Hit!%f"), HealthToMaxRatio);
}

void AAIShootCharacter::Die()
{
	Health = 0;
	Destroy();
}

void AAIShootCharacter::Fire(FVector TargetLocation)
{
	FVector Start, LineTraceEnd, ForwardVector;

	if (IsAiming)
	{

		Start = FollowCamera->GetComponentLocation();

		ForwardVector = (TargetLocation - Start).GetSafeNormal();// FollowCamera->GetForwardVector();
			
		LineTraceEnd = Start + (ForwardVector * 10000);
	}
	else {

		// Get muzzle location
		Start = SK_Gun->GetSocketLocation("MuzzleFlash");

		// Get Rotation Forward Vector
		ForwardVector = FollowCamera->GetForwardVector();

		// Get End Point
		LineTraceEnd = Start + (ForwardVector * 10000);
	}
	Raycast(Start, LineTraceEnd);
}



void AAIShootCharacter::BoostSpeed()
{
	// Set Max walking speed to 800
	GetCharacterMovement()->MaxWalkSpeed = 800.f;

	GetWorld()->GetTimerManager().SetTimer(BoostSpeedTimer, [&]()
		{
			// Set Max walking speed to 500
			GetCharacterMovement()->MaxWalkSpeed = 500.f;
			
			// Clear existing timer boost speed
			GetWorldTimerManager().ClearTimer(BoostSpeedTimer);

		}, 4, false);
}

void AAIShootCharacter::RemoveSpeedBoost()
{
	// Set Max walking speed to 500
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}


void AAIShootCharacter::FireParticle(FVector Start, FVector Impact)
{
	if (!ParticleStart || !ParticleImpact) return;

	FTransform ParticleT;

	ParticleT.SetLocation(Start);

	ParticleT.SetScale3D(FVector(0.25, 0.25, 0.25));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleStart, ParticleT, true);

	// Spawn particle at impact point
	ParticleT.SetLocation(Impact);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleImpact, ParticleT, true);

}

void AAIShootCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AAIShootCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AAIShootCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAIShootCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}




