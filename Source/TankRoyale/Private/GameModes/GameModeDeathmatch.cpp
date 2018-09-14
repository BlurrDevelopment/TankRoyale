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
