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
	
	// TODO Pause game

	TeamOneTanks.Empty();
	TeamTwoTanks.Empty();
	TeamSpectatorTanks.Empty();
}

void AGameModeDeathmatch::Tick(float DeltaTime)
{
	

		Super::Tick(DeltaTime);
		if (!bGameStarted)
		{
			bool bTeamOneReady = false;
			bool bTeamTwoReady = false;

			if (TeamOneTanks.Num() >= TanksPerTeam) bTeamOneReady = true;
			if (TeamTwoTanks.Num() >= TanksPerTeam) bTeamTwoReady = true;

			if (bTeamOneReady && bTeamTwoReady)
			{
				// TODO Countdown from 5
				StartGame();
			}
			else
			{
				// Delay 1 second?
				// TODO UI Displaying game loading
			}
			//UE_LOG(LogTemp, Warning, TEXT("%d"), TeamOneTanks.Num())

				return;
		}
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

		if (TeamOneTanks.Num() <= 0 || TeamTwoTanks.Num() <= 0)
		{
			EndGame();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Winning Team: %i, Team One Score: %i, Team Two Score: %i"), WinningTeam, TeamOneScore, TeamTwoScore);
}

void AGameModeDeathmatch::AssignTankTeam(ATank* Tank)
{
	auto Controller = Tank->GetController();
	
	if (Cast<ATankAIController>(Controller) && TeamTwoTanks.Num() < TanksPerTeam)
	{
		TeamTwoTanks.Add(Tank);
		return;
	}
	else if (Cast<ATankPlayerController>(Controller))
	{
		if (TeamOneTanks.Num() > TeamTwoTanks.Num() && TeamTwoTanks.Num() < TanksPerTeam)
		{
			TeamTwoTanks.Add(Tank);
			return;
		}
		else if (TeamOneTanks.Num() < TanksPerTeam)
		{
			TeamOneTanks.Add(Tank);
			return;
		}
		
	}
	else
	{
		TeamSpectatorTanks.Add(Tank);
		if (Cast<ATankPlayerController>(Controller)) Cast<ATankPlayerController>(Controller)->StartSpectatingOnly();
		Tank->DetachFromControllerPendingDestroy(); // TODO Might cause issues when too many players.
		Tank->DestroyConstructedComponents();
		Tank->Destroy();
		return;
	}
}

void AGameModeDeathmatch::StartGame()
{
	bGameStarted = true;
	GetWorldTimerManager().SetTimer(GameTimerHandler, this, &AGameModeDeathmatch::EndGame, 2.0f, true, 60.0f * GameTime);
	TeamOneScore = 0;
	TeamTwoScore = 0;
	TeamOneKills = 0;
	TeamTwoKills = 0;
	TeamOneDeaths = 0;
	TeamTwoDeaths = 0;
}

void AGameModeDeathmatch::AddTeamDeath(ATank* Tank, ATank* KillerTank)
{
	if (!bGameStarted) return;

	if (TeamOneTanks.Find(Tank) != INDEX_NONE)
	{
		TeamOneDeaths++;
		TeamOneTanks.Remove(Tank);
		// TODO respawn as new tank
		return;
	}
	else if (TeamTwoTanks.Find(Tank) != INDEX_NONE)
	{
		TeamTwoDeaths++;
		TeamTwoTanks.Remove(Tank);
		// TODO respawn as new tank
		return;
	}
}

void AGameModeDeathmatch::EndGame()
{
	if (!bGameStarted) return;

	bGameStarted = false;
	GetWorldTimerManager().ClearAllTimersForObject(this);
	GetWorld()->GetFirstPlayerController()->SetPause(true);
	this->DisplayEndGameUI();
}

int32 AGameModeDeathmatch::GetTankTeam(ATank* Tank) const
{
	if (!ensure(Tank)) return 0;

	if (TeamOneTanks.Find(Tank) != INDEX_NONE) return 1;
	else if (TeamTwoTanks.Find(Tank) != INDEX_NONE) return 2;
	else return 0;
}

int32 AGameModeDeathmatch::GetTeamAlive(int32 Team) const
{
	if (Team == 1)
	{
		return TeamOneTanks.Num();
	}
	else if (Team == 2)
	{
		return TeamTwoTanks.Num();
	}

	return 0;
}

// Receive hits from tanks and check them against tanks
void AGameModeDeathmatch::RegisterTankHit(ATank* ShootingTank, ATank* HitTank)
{
	if (!ensure(ShootingTank)) return;
	if (!ensure(HitTank)) return;
	//if (!(TeamOneTanks.Find(HitTank) != INDEX_NONE) || !(TeamTwoTanks.Find(HitTank) != INDEX_NONE)) return;

	bool bTeamkill = false;
	FString Killer = ShootingTank->GetName();
	FString State = "Unset";
	FString Victim = HitTank->GetName();

	FColor FeedColour;
	if ((TeamOneTanks.Find(ShootingTank) != INDEX_NONE && TeamOneTanks.Find(HitTank) != INDEX_NONE) || (TeamTwoTanks.Find(ShootingTank) != INDEX_NONE && TeamTwoTanks.Find(HitTank) != INDEX_NONE))
	{
		FeedColour = FColor::Red;
		bTeamkill = true;
	}
	else
	{
		FeedColour = FColor::Blue;
		bTeamkill = false;
	}

	if (HitTank->GetHealthPercent() <= 0)
	{
		if (bTeamkill == false)
		{
			if (TeamOneTanks.Find(ShootingTank) != INDEX_NONE) TeamOneKills++;
			if (TeamTwoTanks.Find(ShootingTank) != INDEX_NONE) TeamTwoKills++;
			State = "Killed";
		}
		else State = "Team Killed";
	}
	else State = "Hit";

	this->AddKillToFeed(Killer, State, Victim);

	return;
}

TArray<ATank*> AGameModeDeathmatch::GetTeamTanks(int32 Team) const
{
	if (Team == 1) return TeamOneTanks;
	else return TeamTwoTanks;
}

