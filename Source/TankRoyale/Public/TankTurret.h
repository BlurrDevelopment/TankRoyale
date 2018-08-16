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
	float MaxDegreesPerSecondDamaged = 8.0f;
	
	float StartingDamage = 100;
	float CurrentDamage = StartingDamage;
	float StartDamagedEffects = 50;
	float TotalPart = 0;

	bool bIsDamaged = false;
	bool bIsTotalled = false;
};
