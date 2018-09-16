// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "DeathmatchGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Tank.h"
void AGameModeDeathmatch::PostLogin(APlayerController * NewPlayer) {
	ADeathmatchGameStateBase * GameState = Cast<ADeathmatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->Spawn(NewPlayer, GameState->PlayerNumber);
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
