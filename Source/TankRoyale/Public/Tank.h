// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class APickup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

// Enum for gamemode
UENUM()
enum class EGameMode : uint8
{
	Menu,
	Deathmatch,
	Royale
};

UCLASS()
class TANKROYALE_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Called by the engine when damage is dealt
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Returns current health as a percentage of starting health, between 0 and 100
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UPROPERTY(BlueprintAssignable, Category = "Death")
	FTankDelegate OnDeath;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void SetOnPickup(bool On, APickup* Pickup);

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	APickup* GetCurrentPickup() { return CurrentPickup; }
	void TankDeath(AActor* DamageCauser, int32 DamageToApply);
	void StartGame();
	class AGameModeDeathmatch * AGameMode;
	void Repair(float Amount);
	void SetSpawnPointLocation(FVector Location);
	FVector SpawnPointLocation;
	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnOnServer(TSubclassOf<AActor> ActorToSpawn, FVector SpawnLocation, FRotator SpawnRotation, AController * NewPlayer);
	UPROPERTY(EditDefaultsOnly, Category = "Respawning")
		TSubclassOf<ATank> TankToBe;
		AController * MyController;
		const FName TeamOneTag = "1";
		const FName TeamTwoTag = "2";
		UPROPERTY(BlueprintReadOnly, Category = "Pickups")
			bool bOnPickup;
		UFUNCTION(BlueprintCallable, Category = "Pickups")
			void UsePickup();
		void AsAssignedToTeamSeter(bool Set);
		bool AsAssignedToTeam = false;
protected:

	void PossessedBy(AController * NewController) override;
	UPROPERTY(BlueprintReadOnly, Category = "Pickups")
	APickup* CurrentPickup = nullptr;

private:
	// Sets default values for this pawn's properties
	ATank();

	bool bGameStarted = false;
	
	//UPROPERTY(BlueprintReadOnly, Category = "Gamemode")
	EGameMode GameMode = EGameMode::Menu;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

	bool bDead = false;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<APickup> AmmoPickupBlueprint;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<APickup> HealthPickupBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<APickup> BurstPickupBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UParticleSystem* DeathEmitterTemplate = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UParticleSystem* SmokeEmitterTemplate = nullptr;
	UParticleSystemComponent* SmokeEmitterComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	FVector AmmoOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	FVector HealthOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	FVector BurstOffset;

	void UseAmmoPickup();
	void UseHealthPickup();
	void UseBurstPickup();

	void DropRemainingAmmo();
	void DropHalfAmmo();
	void DropAmmo(int32 Amount);
	void DropHealth(int32 Amount);
	void DropBurst();

	// Sound of the tank exploding
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ExplodeSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ExplodeVolumeMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ExplodePitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ExplodeStartTime = 0.0f;

	// Sound of the tank taking damage
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DamageSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float DamageVolumeMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float DamagePitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float DamageStartTime = 0.0f;


};
