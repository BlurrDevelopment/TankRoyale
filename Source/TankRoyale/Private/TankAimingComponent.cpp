// Copyright Blurr Development 2018.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Tank.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialise(ATank* OwningTank, UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	if (!BarrelToSet)
	{
		// TODO Display error on screen. Code: b1349b04-89e8-11e8-9a94-a6cf71072f73
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
		UE_LOG(LogTemp, Error, TEXT("    Please contact support with the error code."));
		UE_LOG(LogTemp, Error, TEXT("    Discord Support: https://discord.gg/yv3Y25f"));
		UE_LOG(LogTemp, Error, TEXT(" ERROR CODE: b1349b04-89e8-11e8-9a94-a6cf71072f73"));
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
	}

	if (!TurretToSet)
	{
		// TODO Display error on screen. Code: b1349da2-89e8-11e8-9a94-a6cf71072f73
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
		UE_LOG(LogTemp, Error, TEXT("    Please contact support with the error code."));
		UE_LOG(LogTemp, Error, TEXT("    Discord Support: https://discord.gg/yv3Y25f"));
		UE_LOG(LogTemp, Error, TEXT(" ERROR CODE: b1349da2-89e8-11e8-9a94-a6cf71072f73"));
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
	}

	// This should be set by the time initialise is called.
	if (!ProjectileBlueprint)
	{
		// TODO Display error on screen. Code: b13488ee-89e8-11e8-9a94-a6cf71072f73
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
		UE_LOG(LogTemp, Error, TEXT("    Please contact support with the error code."));
		UE_LOG(LogTemp, Error, TEXT("    Discord Support: https://discord.gg/yv3Y25f"));
		UE_LOG(LogTemp, Error, TEXT(" ERROR CODE: b13488ee-89e8-11e8-9a94-a6cf71072f73"));
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
	}
	if (!ensure(OwningTank)) return;

	Barrel = BarrelToSet;
	Turret = TurretToSet;
	OwnerTank = OwningTank;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (RoundsLeft <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();

	return !BarrelForward.Equals(AimDirection, 0.01f);
}


void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation, HitLocation, LaunchSpeed, false, 0.0f, 0.0f, ESuggestProjVelocityTraceOption::DoNotTrace);

	if (bHaveAimSolution)
	{
	    AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}

	// If no velocity found, do nothing
	return;
}

// Move the tanks barrel.
void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	// Work out difference between current barrel rotation and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);

	// Always yaw the shortest way
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel) || !ensure(ProjectileBlueprint)) { return; }

	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{

		// Spawn a projectile at the socket location on the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));

		// TODO Set owning tank of projectile
		Projectile->SetFiringTank(OwnerTank);

		// Launch the projectile
		Projectile->LaunchProjectile(LaunchSpeed);

		// Set the last fire time
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;

		// Play fire sound
		if (!ensure(FireSound)) return;
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Barrel->GetSocketLocation(FName("Projectile")), FireVolumeMultiplier, FirePitchMultiplier, FireStartTime);

		// Start the timer for destroying the projectile
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &UTankAimingComponent::OnReload, ReloadTimeInSeconds, false);
	}
	else if (FiringState == EFiringState::OutOfAmmo || FiringState == EFiringState::Reloading)
	{
		// Play empty barrel sound
		if (!ensure(EmptySound)) return;
		UGameplayStatics::PlaySoundAtLocation(this, EmptySound, Barrel->GetSocketLocation(FName("Projectile")), EmptyVolumeMultiplier, EmptyPitchMultiplier, EmptyStartTime);
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

void UTankAimingComponent::OnReload()
{
	// Play reload sound
	if (!ensure(ReloadSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, Barrel->GetSocketLocation(FName("Projectile")), ReloadVolumeMultiplier, ReloadPitchMultiplier, ReloadStartTime);
}