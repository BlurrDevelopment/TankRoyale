// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeDeathmatch.generated.h"



/**
 * 
 */
UCLASS()
class TANKROYALE_API AGameModeDeathmatch : public AGameModeBase
{
	GENERATED_BODY()
public:
	AActor * SpawnActor(TSubclassOf<AActor> ActorToSpawn, FVector SpawnLocation, FRotator SpawnRotation);
	void SpawnAI();
protected:
	void PostLogin(APlayerController * NewPlayer) override;
private:
	class ADeathmatchGameStateBase * GameState;
	bool bHaveSpawnedAi = false;
 };


