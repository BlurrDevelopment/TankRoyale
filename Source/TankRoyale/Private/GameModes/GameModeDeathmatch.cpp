// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "Tank.h"
#include "TankAIController.h"
#include "TankPlayerController.h"
#include "TimerManager.h"

AGameModeDeathmatch::AGameModeDeathmatch()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AGameModeDeathmatch::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(GameTimerHandler, this, &AGameModeDeathmatch::OnEndGame, 2.0f, true, 60.0f * GameTime);
}

void AGameModeDeathmatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TeamOneDeaths == TeamTwoDeaths)
	{
		WinningTeam = 0;
	}
	else if (TeamOneDeaths > TeamTwoDeaths)
	{
		WinningTeam = 1;
	}
	else if (TeamOneDeaths < TeamTwoDeaths)
	{
		WinningTeam = 2;
	}
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

void AGameModeDeathmatch::AddTeamDeath(ATank* Tank)
{
	if (TeamOneTanks.Find(Tank) != INDEX_NONE)
	{
		// Remove the tank from the team and increment the death toll.
		TeamOneTanks.Remove(Tank);
		TeamOneDeaths++;

		// TODO: respawn as new tank

		return;
	}
	else if (TeamTwoTanks.Find(Tank) != INDEX_NONE)
	{
		// Remove the tank from the team and increment the death toll.
		TeamTwoTanks.Remove(Tank);
		TeamTwoDeaths++;

		// TODO: respawn as new tank

		return;
	}
}

void AGameModeDeathmatch::OnEndGame()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
}