// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "DeathmatchGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Menus/WidgetGameTypeManager.h"
#include "GameInstances/NetworkGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
void AGameModeDeathmatch::SpawnAI()
{
	if (!bHaveSpawnedAi)
	{
	int16 TeamOne =	Cast<UNetworkGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetTeam1AI();
	int16 TeamTwo = Cast<UNetworkGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetTeam2AI();
		bHaveSpawnedAi = true;
		for (size_t i = 0; i < TeamOne; i++)
		{
			ATank * Tank = Cast<ATank>(GameState->SpawnAi(i));
GameState->AssignTankToTeamByN(1, Tank);
Tank->AsAssignedToTeamSeter(true);
		}
		for (size_t i = 0; i < TeamTwo; i++)
		{
			int16 SpawnNum = (i + 5);
			ATank * Tank = Cast<ATank>(GameState->SpawnAi(SpawnNum));
			GameState->AssignTankToTeamByN(2, Tank);
			Tank->AsAssignedToTeamSeter(true);
		}
	}
}

void AGameModeDeathmatch::PostLogin(APlayerController * NewPlayer) {
	GameState = Cast<ADeathmatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->Spawn(NewPlayer, GameState->PlayerNumber);
	SpawnAI();
}
AActor * AGameModeDeathmatch::SpawnActor(TSubclassOf<AActor> ActorToSpawn, FVector SpawnLocation, FRotator SpawnRotation)
{
	UWorld * World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null"));
	}
	return World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation);
}
