// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "GunSurvivorsGameMode.h"

AGunSurvivorsGameMode::AGunSurvivorsGameMode()
{

    // Allow the actor to tick
    PrimaryActorTick.bCanEverTick = true;
}

void AGunSurvivorsGameMode::BeginPlay()
{

    // Call the parent class's BeginPlay()
    Super::BeginPlay();

    // Set the score to 0
    SetScore(0);
}

void AGunSurvivorsGameMode::SetScore(int NewScore)
{

    // Check if the new score is valid
    if(NewScore >= 0) 
    {

        // Set the score
        Score = NewScore;

        // Broadcast the score changed delegate
        ScoreChangedDelegate.Broadcast(Score);
    }
}

void AGunSurvivorsGameMode::AddScore(int AmountToScore)
{

    // Calculate the new score
    int NewScore = Score + AmountToScore;

    // Set the score
    SetScore(NewScore);
}

void AGunSurvivorsGameMode::RestartGame()
{

    // Start the timer to restart the level
    GetWorldTimerManager().SetTimer(RestartGameTimer, this, &AGunSurvivorsGameMode::OnRestartGameTimerTimeout, 1.0f, false, TimeBeforeRestart);
}

void AGunSurvivorsGameMode::OnRestartGameTimerTimeout()
{

    // Open the main level
    UGameplayStatics::OpenLevel(GetWorld(), FName("L_Main"));
}
