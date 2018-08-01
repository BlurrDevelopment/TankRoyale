// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "Tank.h"
#include "TankAIController.h"
#include "TankPlayerController.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

AGameModeDeathmatch::AGameModeDeathmatch()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AGameModeDeathmatch::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(GameTimerHandler, this, &AGameModeDeathmatch::OnEndGame, 2.0f, true, 60.0f * GameTime);
	TeamOneScore = 0;
	TeamTwoScore = 0;
	TeamOneKills = 0;
	TeamTwoKills = 0;
	TeamOneDeaths = 0;
	TeamTwoDeaths = 0;
	TeamOneTanks.Empty();
	TeamTwoTanks.Empty();
	TeamSpectatorTanks.Empty();
}

void AGameModeDeathmatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TeamOneScore = TeamOneKills - TeamOneDeaths;
	TeamTwoScore = TeamTwoKills - TeamTwoDeaths;

	if (TeamOneScore == TeamTwoScore)
	{
		WinningTeam = 0;
	}
	else if (TeamOneScore > TeamTwoScore)
	{
		WinningTeam = 1;
	}
	else if (TeamOneScore < TeamTwoScore)
	{
		WinningTeam = 2;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Winning Team: %i, Team One Score: %i, Team Two Score: %i"), WinningTeam, TeamOneScore, TeamTwoScore);
}

void AGameModeDeathmatch::AssignTankTeam(ATank* Tank)
{
	auto Controller = Tank->GetController();
	
	if (Cast<ATankAIController>(Controller) && TeamTwoTanks.Num() < 5)
	{
		TeamTwoTanks.Add(Tank);
		return;
	}
	else if (Cast<ATankPlayerController>(Controller) && TeamOneTanks.Num() < 5)
	{
		TeamOneTanks.Add(Tank);
		return;
	}
	else
	{
		TeamSpectatorTanks.Add(Tank);
		if (Cast<ATankPlayerController>(Controller)) Cast<ATankPlayerController>(Controller)->StartSpectatingOnly();
		Tank->DetachFromControllerPendingDestroy(); // TODO: Might cause issues when too many players.
		Tank->DestroyConstructedComponents();
		Tank->Destroy();
		return;
	}
}

void AGameModeDeathmatch::AddTeamDeath(ATank* Tank, ATank* KillerTank)
{
	if (TeamOneTanks.Find(Tank) != INDEX_NONE)
	{
		// Remove the tank from the team and increment the death toll.
		TeamOneTanks.Remove(Tank);
		TeamOneDeaths++;
		if (TeamTwoTanks.Find(KillerTank)) TeamTwoKills++;

		// TODO: respawn as new tank

		return;
	}
	else if (TeamTwoTanks.Find(Tank) != INDEX_NONE)
	{
		// Remove the tank from the team and increment the death toll.
		TeamTwoTanks.Remove(Tank);
		TeamTwoDeaths++;
		if (TeamOneTanks.Find(KillerTank)) TeamOneKills++;

		// TODO: respawn as new tank

		return;
	}
}

void AGameModeDeathmatch::OnEndGame()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	GetWorld()->GetFirstPlayerController()->SetPause(true);
	//UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
	this->DisplayEndGameUI();
}