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
class ATankGadget;

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

	void StartGame();

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void FireGadget();

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetRoundsLeft() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetRoundsLoaded() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetMaxRounds() const { return MaxRounds; }

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddAmmo(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void LoadSpecialAmmo();

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetGadgetsLeft() const { return GadgetsLeft; }

	UFUNCTION(BlueprintCallable, Category = "Heat")
	float GetHeat() { return CurrentHeat; }

	UFUNCTION(BlueprintCallable, Category = "Heat")
	float GetHeatPerShot() { return HeatPerShot; }

	UFUNCTION(BlueprintCallable, Category = "Heat")
	bool IsOverheated() { return bOverheated; }

	/// Disable Aiming
	void Disable(float Time);
	void OnEnable();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Aiming;

private:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsBarrelMoving();

	void MoveBarrelTowards(FVector AimDirection);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	float GetLaunchSpeed() { return LaunchSpeed; }

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void IncreaseLaunchSpeed();

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void DecreaseLaunchSpeed();

	UTankBarrel * Barrel = nullptr;
	UTankTurret* Turret = nullptr;
	ATank* OwnerTank = nullptr;
	ATankGadget* Gadget = nullptr;

	FVector AimDirection;

	// The projectile the tank uses.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	// The gadget the tank uses.
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	TSubclassOf<ATankGadget> GadgetBlueprint;

	// The maximum speed of the projectile is launched.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float MinLaunchSpeed = 4500;

	// The minimum speed of the projectile is launched.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float MaxLaunchSpeed = 6500;

	// The speed of the projectile is launched.
	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float LaunchSpeed = MaxLaunchSpeed;

	// How long it takes to reload.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3.0f;

	// How long between each shot for burst fire.
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float BurstTime = 0.15f;

	// How many rounds the tank has left.
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 RoundsLeft = 24;

	// Max amount of rounds the tank can have.
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxRounds = 24;

	// How many rounds the tank has left.
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 GadgetsLeft = 1;

	// Max amount of rounds the tank can have.
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxGadgets = 1;

	// How many rounds are loaded.
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 RoundsLoaded = 1;

	// Max amount of rounds that can be loaded.
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxRoundsLoadable = 1;

	// Max amount of rounds that can be loaded for burst by special pickup.
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxSpecialRoundsLoadable = 3;

	void FireRound();
	void Reload();

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

	/// HEAT
	// How much heat per shot
	UPROPERTY(EditDefaultsOnly, Category = "Heat")
	float HeatPerShot = 55.0f;

	// Heat to overheat at
	UPROPERTY(EditDefaultsOnly, Category = "Heat")
	float OverheatAt = 100.0f;

	// Decrease per second
	UPROPERTY(EditDefaultsOnly, Category = "Heat")
	float HeatLossPerSecond = 10.0f;

	float CurrentHeat = 0.0f;
	bool bOverheated = false;

	/// Disable Aiming
	bool bAimingDisabled = false;
	bool bGameStarted = false;
};