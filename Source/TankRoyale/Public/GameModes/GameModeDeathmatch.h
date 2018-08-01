// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeDeathmatch.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class TANKROYALE_API AGameModeDeathmatch : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	AGameModeDeathmatch();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	int32 TeamOneScore = 0;
	int32 TeamOneKills = 0;
	int32 TeamOneDeaths = 0;

	int32 TeamTwoScore = 0;
	int32 TeamTwoKills = 0;
	int32 TeamTwoDeaths = 0;

	int32 WinningTeam = 0;

	TArray<ATank*> TeamOneTanks;
	TArray<ATank*> TeamTwoTanks;
	TArray<ATank*> TeamSpectatorTanks;

public:
	void AssignTankTeam(ATank* Tank);

	void AddTeamDeath(ATank* Tank);

	// How long a round lasts in minutes.
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	float GameTime = 10.0f;

	// How many rounds in a game.
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int32 GameRounds = 1;

	FTimerHandle GameTimerHandler;

	void OnEndGame();
	
};
