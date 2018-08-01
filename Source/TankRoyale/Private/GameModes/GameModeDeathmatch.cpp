// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "Tank.h"
#include "TankAIController.h"
#include "TankPlayerController.h"

void AGameModeDeathmatch::StartPlay()
{
	Super::StartPlay();
}

void AGameModeDeathmatch::AssignTankTeam(ATank* Tank)
{
	auto Controller = Tank->GetController();
	
	if (Cast<ATankAIController>(Controller))
	{
		TeamTwoTanks.Add(Tank);
		UE_LOG(LogTemp, Warning, TEXT("Tank added to team two, it is AI."));
		return;
	}
	if (Cast<ATankPlayerController>(Controller))
	{
		TeamOneTanks.Add(Tank);
		UE_LOG(LogTemp, Warning, TEXT("Tank added to team one, it is a player."));
	}
}

void AGameModeDeathmatch::AddTeamDeath(ATank* Tank)
{
	if (TeamOneTanks.Find(Tank) != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("A member of team one died."));
		
		// Remove the tank from the team and increment the death toll.
		TeamOneTanks.Remove(Tank);
		TeamOneDeaths++;

		// TODO: respawn as new tank

		return;
	}
	if (TeamTwoTanks.Find(Tank) != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("A member of team two died."));

		TeamTwoTanks.Remove(Tank);
		TeamTwoDeaths++;

		return;
	}
}