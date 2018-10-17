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
	UPROPERTY(EditAnywhere)
	float Timer = 3;
protected:
	void PostLogin(APlayerController * NewPlayer) override;
private:
	class ADeathmatchGameStateBase * GameState;
	bool bHaveSpawnedAi = false;
	UFUNCTION()
		void OnTimerEnd();
	UFUNCTION()
		void OnTimerEndTeam2();
	int16 PlayerNum = 0;
	int16 PlayerNumTeam2 = 0;
	FTimerHandle TimerHandle;
	int16 TeamTwo;
	int16 TeamOne;
 };


