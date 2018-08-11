// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class AAmmoPickup;

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
	void SetOnPickup(bool On, AAmmoPickup* Pickup);

	UFUNCTION(BlueprintCallable, Category = "Pickups")
	AAmmoPickup* GetCurrentPickup() { return CurrentPickup; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pickups")
		bool bOnPickup;

	UPROPERTY(BlueprintReadOnly, Category = "Pickups")
		AAmmoPickup* CurrentPickup = nullptr;

private:
	// Sets default values for this pawn's properties
	ATank();

	//UPROPERTY(BlueprintReadOnly, Category = "Gamemode")
	EGameMode GameMode = EGameMode::Menu;

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;

	bool bDead = false;

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
