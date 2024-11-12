// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP3ShootGameMode.h"
#include "TP3ShootCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATP3ShootGameMode::ATP3ShootGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Not used
void ATP3ShootGameMode::RespawnPlayer()
{
	// Récupérer le PlayerController (index 0 pour le joueur principal)
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		// Définir la position de spawn (remplacez par la position souhaitée)
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(1980.0f, 3090.0f, 90.0f)); // Remplacez par votre emplacement de respawn
        
		// Charger la classe du Blueprint
		static ConstructorHelpers::FObjectFinder<UClass> PlayerCharacterBP(TEXT("Blueprint'/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter'"));
        
		if (PlayerCharacterBP.Succeeded())
		{
			// Faire apparaître le personnage à partir du Blueprint
			AActor* NewPlayerCharacter = GetWorld()->SpawnActor<AActor>(PlayerCharacterBP.Object, SpawnTransform);
			if (NewPlayerCharacter)
			{
				// Posséder le nouveau personnage
				PlayerController->Possess(Cast<APawn>(NewPlayerCharacter));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load Player Character Blueprint!"));
		}
	}
}
