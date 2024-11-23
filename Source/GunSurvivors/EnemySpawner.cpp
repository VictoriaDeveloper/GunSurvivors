// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::BeginPlay()
{

	// Call the parent class' BeginPlay()
	Super::BeginPlay();
	
	// Hold the game mode
	AGameModeBase *GameMode = UGameplayStatics::GetGameMode(GetWorld());
	
	// Check if the game mode is not null
	if (GameMode != nullptr)
	{

		// Cast the game mode to AGunSurvivorsGameMode
		MyGameMode = Cast<AGunSurvivorsGameMode>(GameMode);

		// Check if the game mode was casted correctly
		check(MyGameMode);
	}


	// Get the player actor
	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCharacter::StaticClass());
	if (PlayerActor)
	{

		// Cast the actor to ATopdownCharacter
		PlayerCharacter = Cast<ATopdownCharacter>(PlayerActor);

		// Bind the delegate to the player when they die
		PlayerCharacter->PlayerDiedDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
	}

	// Start spawning
	StartSpawning();
}

void AEnemySpawner::Tick(float DeltaTime)
{

	// Call the parent class' Tick()
	Super::Tick(DeltaTime);

}

void AEnemySpawner::OnSpawnTimerTimeout()
{

	// Spawn an enemy
	SpawnEnemy();
}

void AEnemySpawner::StartSpawning()
{	

	// Set the timer to spawn an enemy
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, SpawnTime, true, SpawnTime);
}

void AEnemySpawner::StopSpawning()
{

	// Clear the timer 
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::SpawnEnemy()
{

	// Hold the random position in the unity circle
	FVector2D RandomPosition = FVector2D(FMath::VRand());

	// Normalize the position
	RandomPosition.Normalize();

	// Multiply the vector by the spawn distance
	RandomPosition *= SpawnDistance;

	// Hold the actor location to spawn the enemy
	FVector EnemyLocation = GetActorLocation() + FVector(RandomPosition.X, 0.0f, RandomPosition.Y);

	// Spawn the AEnemy actor
	AEnemy* EnemySpawned = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemyLocation, 
	FRotator::ZeroRotator);

	// Setup the enemy spawned
	SetupEnemy(EnemySpawned);

	// Increment the total enemy count by 1
	TotalEnemyCount++;

	// Check if we reached a difficulty spike interval
	if(TotalEnemyCount % DifficultySpikeInterval == 0)
	{

		// Check if we can decrease the spawn time
		if(SpawnTime > SpawnTimeMinimumLimit)
		{

			// Decrease the spawn time
			SpawnTime -= DecreaseSpawnTimerByEveryInterval;
			
			// Check if the spawn time went below the minimum spawn time
			if(SpawnTime < SpawnTimeMinimumLimit)
			{

				// Set the new spawn time
				SpawnTime = SpawnTimeMinimumLimit;
			}

			// Stop to reset the spawning
			StopSpawning();

			// Reset the the spawning
			StartSpawning();
		}
	}
}

void AEnemySpawner::SetupEnemy(AEnemy *EnemyCharacter)
{

	// Check if there is an enemy character
	if (EnemyCharacter)
	{

		// Set the player character for the enemy
		EnemyCharacter->PlayerCharacter = PlayerCharacter;

		// Allow the enemy to follow
		EnemyCharacter->CanFollow = true;

		// Add this function the enemy died delgate to be fired when it broadcasts
		EnemyCharacter->EnemyDiedDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);
	}
}

void AEnemySpawner::OnEnemyDied()
{

	// Hold the score to add
	int ScoreToAdd = 10;

	// Add the score to the game mode
	MyGameMode->AddScore(10);
}

void AEnemySpawner::OnPlayerDied()
{

	// Stop enemies from spawning
	StopSpawning();

	// Hold all the enemies
	TArray<AActor*> EnemyArray;

	// Set all the all actors of the Enemy class
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyArray);

	// Loop through the Enemy actors
	for (AActor *EnemyActor : EnemyArray)
	{

		// Hold the enemy 
		AEnemy* EnemyCharacter = Cast<AEnemy>(EnemyActor);
		
		// Check if the enemy is not nullptr and is alive
		if (EnemyCharacter && EnemyCharacter->IsAlive)
		{

			// Set CanFollow to false
			EnemyCharacter->CanFollow = false;
		}
	}


	// Restart the game
	MyGameMode->RestartGame();
}
