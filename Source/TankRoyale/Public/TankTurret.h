// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))//, hidecategories = ("Collision"))
class TANKROYALE_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Rotate(float RelativeSpeed);
	
	float TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Damage")
	float GetTurretDamage() const; 

	UFUNCTION(BlueprintPure, Category = "Damage")
	bool IsTurretDamaged() const;

	UFUNCTION(BlueprintPure, Category = "Damage")
	bool IsTurretTotalled() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecond = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecondDamaged = 18.5f;

	// The starting and max health of the tank part.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float StartingDamage = 125;

	float CurrentDamage = StartingDamage;

	// The point when the tank part starts to have side effects and malfunctioning.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float StartDamagedEffects = 50;

	// The point when the tank part loses most of its functions.
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float TotalPart = 0;

	bool bIsDamaged = false;
	bool bIsTotalled = false;
};
