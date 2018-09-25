// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "DeathmatchGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Menus/WidgetGameTypeManager.h"
#include "Tank.h"
void AGameModeDeathmatch::SpawnAI(AController * Controller)
{
	if (!bHaveSpawnedAi)
	{
		bHaveSpawnedAi = true;
		for (size_t i = 0; i < Menu->TeamOneAi; i++)
		{
ATank * Tank = Cast<ATank>(GameState->Spawn(Controller, 5 - (Menu->TeamOneAi - i)));
GameState->AssignTankToTeamByN(1, Tank);
Tank->AsAssignedToTeamSeter(true);
		}
		for (size_t i = 0; i < Menu->TeamTwoAi; i++)
		{
			ATank * Tank = Cast<ATank>(GameState->Spawn(Controller, 10 - (Menu->TeamTwoAi + i)));
			GameState->AssignTankToTeamByN(2, Tank);
			Tank->AsAssignedToTeamSeter(true);
		}
	}
}
void AGameModeDeathmatch::Setwidget(UWidgetGameTypeManager * WidgetToSet)
{
	Menu = WidgetToSet;
}
void AGameModeDeathmatch::PostLogin(APlayerController * NewPlayer) {
	ADeathmatchGameStateBase * GameState = Cast<ADeathmatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->Spawn(NewPlayer, GameState->PlayerNumber);
	//SpawnAI(ControllerSubClass.GetDefaultObject());
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
