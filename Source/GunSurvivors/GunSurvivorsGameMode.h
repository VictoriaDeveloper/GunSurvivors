// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TimerHandle.h"
#include "GunSurvivorsGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChangedDelegate, int, NewScore);

/**
 * 
 */
UCLASS()
class GUNSURVIVORS_API AGunSurvivorsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Hold the score 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Score = 0;

	// Hold the score changed delegate
	UPROPERTY(BlueprintAssignable)
	FScoreChangedDelegate ScoreChangedDelegate;

	// Hold the float that holds the time before restart
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBeforeRestart = 0.3f;

	// Hold the restart game timer
	FTimerHandle RestartGameTimer;

	/**
	 * @brief Constructor for the game mode 
	 * @param none
	 * @return AGunSurvivorsGameMode
	 */
	AGunSurvivorsGameMode();

	/**
	 * @brief Handle the start of the game mode by setting the score to 0
	 * @param none
	 * @return void
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Set the score for the game mode
	 * @param int NewScore
	 * @return void
	 */
	void SetScore(int NewScore);

	/**
	 * @brief Add the score to the game mode
	 * @param int AmountToScore
	 * @return void
	 */
	void AddScore(int AmountToScore);

	/**
	 * @brief Handle restarting the game by starting the timer
	 * @param none
	 * @return void
	 */
	void RestartGame();

	/**
	 * @brief Restart the game once the timer timesout
	 * @param none
	 * @return void
	 */
	void OnRestartGameTimerTimeout();

};
