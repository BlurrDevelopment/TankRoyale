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

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void SetOnPickup(bool On, APickup* Pickup);  // TODO Change to APickup

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	APickup* GetCurrentPickup() { return CurrentPickup; }  // TODO Change to APickup

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pickups")
	bool bOnPickup;

	UPROPERTY(BlueprintReadOnly, Category = "Pickups")
	APickup* CurrentPickup = nullptr;  // TODO Change to APickup

private:
	// Sets default values for this pawn's properties
	ATank();

	void TankDeath(AActor* DamageCauser, int32 DamageToApply);

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

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UParticleSystem* EmitterTemplate = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void UsePickup();
	void UseAmmoPickup();
	void UseHealthPickup();

	void DropRemainingAmmo();
	void DropHalfAmmo();

	void DropHalfHealth();

	// Sound of the tank exploding
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ExplodeSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ExplodeVolumeMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ExplodePitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ExplodeStartTime = 0.0f;
};
