// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming state
UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

// Forward declaration
class UTankBarrel;
class UTankTurret;
class AProjectile;
class ATank;

// Holds barrels parameters and elavate method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKROYALE_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(ATank* OwningTank, UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRoundsLeft() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddAmmo(int32 Amount);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

private:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsBarrelMoving();

	void MoveBarrelTowards(FVector AimDirection);

	UTankBarrel * Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	ATank* OwnerTank = nullptr;

	double LastFireTime = 0.0;

	FVector AimDirection;

	// The projectile the tank uses.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	// The speed of the projectile is launched.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 6500;

	// How long it takes to reload.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3.0f;

	// How many rounds the tank has left.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 RoundsLeft = 21;

	void OnReload();

	// Sound of the tank firing
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* FireSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float FireVolumeMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float FirePitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float FireStartTime = 0.0f;

	// Sound of the tank reloading
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ReloadSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ReloadVolumeMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ReloadPitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float ReloadStartTime = 0.0f;

	// Sound of the tank empty
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* EmptySound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float EmptyVolumeMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float EmptyPitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float EmptyStartTime = 0.0f;
};