// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))//, hidecategories = ("Collision"))
class TANKROYALE_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// -1 is max downward movment, 1 is max upward movement
	void Elevate(float RelativeSpeed);

	float TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Damage")
	float GetBarrelDamage() const;

	UFUNCTION(BlueprintPure, Category = "Damage")
	bool IsBarrelDamaged() const;

	UFUNCTION(BlueprintPure, Category = "Damage")
	bool IsBarrelTotalled() const;

	UFUNCTION(BlueprintPure, Category = "Damage")
	bool CanBarrelFire() const;

	void SetOverheated(bool Overheated) { bOverheated = Overheated; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecond = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecondDamaged = 2.8f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxElevationDegrees = 35.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinElevationDegrees = 0.0f;
	
	// DAMAGE
	// The starting and max health of the tank part.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float StartingDamage = 125;

	float CurrentDamage = StartingDamage;

	// The point when the tank part starts to have side effects and malfunctioning.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float StartDamagedEffects = 75;

	// The point when the tank part loses most of its functions.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float TotalPart = 0;

	bool bIsDamaged = false;
	bool bIsTotalled = false;
	bool bCanFire = true;

	// HEAT
	bool bOverheated = false;

};
