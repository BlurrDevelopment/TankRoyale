// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DeathmatchGameStateBase.generated.h"

/**
 * 
 */
class ATank;
UCLASS()
class TANKROYALE_API ADeathmatchGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	ADeathmatchGameStateBase();

	virtual void BeginPlay() override;
	void WaitForPlayers();
	virtual void Tick(float DeltaTime) override;

	int32 TeamOneScore = 0;
	int32 TeamOneKills = 0;
	int32 TeamOneDeaths = 0;

	int32 TeamTwoScore = 0;
	int32 TeamTwoKills = 0;
	int32 TeamTwoDeaths = 0;

	int32 WinningTeam = 0;

	bool bGameStarted = false;

	TArray<ATank*> TeamOneTanks;
	TArray<ATank*> TeamTwoTanks;
	TArray<ATank*> TeamSpectatorTanks;

public:
	int16 PlayerNumber = 0;

	UFUNCTION(BlueprintCallable, Category = "Start")
	bool HasGameStarted() { return bGameStarted; }

	void AssignTankTeam(ATank* Tank);

	UFUNCTION(BlueprintCallable, Category = "Start")
	void StartGame();

	void AddTeamDeath(ATank* Tank, ATank* KillerTank);

	// How long a round lasts in minutes.
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	float GameTime = 10.0f;

	// How many rounds in a game.
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int32 GameRounds = 1;

	// How many tanks per team. Default: 5
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
	int32 TanksPerTeam = 5;

	FTimerHandle GameTimerHandler;

	// Get team ones score.
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetTeamOneScore() const { return TeamOneScore; }

	// Get team twos score.
	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetTeamTwoScore() const { return TeamTwoScore; }

	// Get which team the tank is on.
	UFUNCTION(BlueprintPure, Category = "Team")
	int32 GetTankTeam(ATank* Tank) const;

	// Get how many members of the team are alive.
	UFUNCTION(BlueprintPure, Category = "Team")
	int32 GetTeamAlive(int32 Team) const;

	// Get all the tanks in the team.
	UFUNCTION(BlueprintPure, Category = "Team")
	TArray<ATank*> GetTeamTanks(int32 Team) const;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATank> TankSubClass;
	
	UPROPERTY()
	class ATankSpawnPoint * PointToSpawn;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "EndGame")
	void DisplayEndGameUI();

	void EndGame();
	void RegisterTankHit(ATank* ShootingTank, ATank* HitTank);

	UFUNCTION(BlueprintImplementableEvent, Category = "Killfeed")
	void AddKillToFeed(const FString& Killer, const FString& State, const FString& Victim);//FString Killer, FString State, FString Victim               const FString& Killer, const FString& State, const FString& Victim
	
	bool Timer = false;

	AActor * Spawn(AController * NewPlayer, int16 SpawnPointNumber);

	void AssignTankToTeamByN(int16 TeamN, ATank * Tank);

	AActor * SpawnAi(int16 SpawnPointNumber);

	TSubclassOf<class ATankSpawnPoint> SpawnPoint;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	TArray<	FVector> SapwnPointLocation;

	void CreateTankSpawnPoints();
private:
	TArray<class ATankSpawnPoint*> ATankSpawnPoints;
	void Respawn(AController * NewPlayer, FVector SpawnLocation);

};
