// Copyright Blurr Development 2018.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Tank.h"
#include "TankGadget.h"
#include "Engine/World.h"
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
	//TODO for testing
	bGameStarted = true;
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

void UTankAimingComponent::StartGame()
{
	//TODO for testing
	bGameStarted = true;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Set the firing state
	if ((RoundsLeft <= 0 && RoundsLoaded <= 0) || !Barrel->CanBarrelFire() || bOverheated || bAimingDisabled || !bGameStarted)
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else if (RoundsLoaded < MaxRoundsLoadable)
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

	RoundsLeft = FMath::Clamp<int32>(RoundsLeft, 0, MaxRounds);

	// Set the barrel overheat bool
	if (CurrentHeat >= OverheatAt)
	{
		Barrel->SetOverheated(true);
		bOverheated = true;
	}
	else if (CurrentHeat <= 10)
	{
		bOverheated = false;
	}
	else
	{
		Barrel->SetOverheated(false);
	}

	// Heat loss
	auto HeatChange = HeatLossPerSecond * DeltaTime;
	CurrentHeat = FMath::Clamp<float>((CurrentHeat - HeatChange), 0.0f, 10000);
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();

	return !BarrelForward.Equals(AimDirection, 0.01f);
}


void UTankAimingComponent::AimAt(FVector HitLocation)
{
	// Dont shoot if the game hasnt started
	if (!bGameStarted) return;

	if (!ensure(Barrel) ||  !ensure(Turret)) { return; }

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
	// Dont shoot if the game hasnt started
	if (!bGameStarted) return;

	if (!ensure(Barrel) || !ensure(Turret)) { return; }
	if (bAimingDisabled) return;

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
	// Dont shoot if the game hasnt started
	if (!bGameStarted) return;

	if (!ensure(Barrel) || !ensure(ProjectileBlueprint)) { return; }
	if (bAimingDisabled) return;

	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		FiringState = EFiringState::Reloading;
		FireRound();
	}
	else if (FiringState == EFiringState::OutOfAmmo || FiringState == EFiringState::Reloading)
	{
		// Play empty barrel sound
		if (!ensure(EmptySound)) return;
		UGameplayStatics::PlaySoundAtLocation(this, EmptySound, Barrel->GetSocketLocation(FName("Projectile")), EmptyVolumeMultiplier, EmptyPitchMultiplier, EmptyStartTime);
	}
}

void UTankAimingComponent::FireGadget()
{
	// Dont shoot if the game hasnt started
	if (!bGameStarted) return;

	if (!ensure(Barrel) || !ensure(GadgetBlueprint)) { return; }
	if (bAimingDisabled) return;

	if (GadgetsLeft > 0)
	{
		Gadget = GetWorld()->SpawnActor<ATankGadget>(GadgetBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
		Gadget->LaunchGadget(LaunchSpeed);
		if (!ensure(FireSound)) return;
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Barrel->GetSocketLocation(FName("Projectile")), FireVolumeMultiplier, FirePitchMultiplier, FireStartTime);
		GadgetsLeft--;
	}
	else
	{
		// Play empty barrel sound
		if (!ensure(EmptySound)) return;
		UGameplayStatics::PlaySoundAtLocation(this, EmptySound, Barrel->GetSocketLocation(FName("Projectile")), EmptyVolumeMultiplier, EmptyPitchMultiplier, EmptyStartTime);
	}
}

void UTankAimingComponent::FireRound()
{
	// Dont shoot if the game hasnt started
	if (!bGameStarted) return;

	// Spawn a projectile at the socket location on the barrel
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));

	// Set owning tank of projectile
	Projectile->SetFiringTank(OwnerTank);

	// Launch the projectile
	Projectile->LaunchProjectile(LaunchSpeed);

	// Play fire sound
	if (!ensure(FireSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, Barrel->GetSocketLocation(FName("Projectile")), FireVolumeMultiplier, FirePitchMultiplier, FireStartTime);

	// Increase heat
	CurrentHeat = CurrentHeat + HeatPerShot;

	// Decrease loaded rounds
	RoundsLoaded--;
	GetOwner()->MakeNoise();
	if (RoundsLoaded > 0)
	{
		// Reload another round
		FTimerHandle BurstTimer;
		GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &UTankAimingComponent::FireRound, BurstTime, false);
	}
	else
	{
		// Reload
		FTimerHandle ReloadTimer;
		float ReloadTime = ReloadTimeInSeconds / MaxRoundsLoadable;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &UTankAimingComponent::Reload, ReloadTime, false);
	}
}

void UTankAimingComponent::Reload()
{
	RoundsLoaded++;
	RoundsLeft--;

	if (!ensure(ReloadSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, Barrel->GetSocketLocation(FName("Projectile")), ReloadVolumeMultiplier, ReloadPitchMultiplier, ReloadStartTime);
	
	if (RoundsLoaded < MaxRoundsLoadable)
	{
		// Reload another round
		FTimerHandle ReloadTimer;
		float ReloadTime = ReloadTimeInSeconds / MaxRoundsLoadable;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &UTankAimingComponent::Reload, ReloadTime, false);
	}
}

void UTankAimingComponent::LoadSpecialAmmo()
{
	RoundsLoaded++;
	RoundsLeft--;

	if (!ensure(ReloadSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, Barrel->GetSocketLocation(FName("Projectile")), ReloadVolumeMultiplier, ReloadPitchMultiplier, ReloadStartTime);

	if (RoundsLoaded < MaxSpecialRoundsLoadable)
	{
		// Reload another round
		FTimerHandle ReloadTimer;
		float ReloadTime = ReloadTimeInSeconds / MaxSpecialRoundsLoadable;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &UTankAimingComponent::LoadSpecialAmmo, ReloadTime, false);
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

int32 UTankAimingComponent::GetRoundsLoaded() const
{
	return RoundsLoaded;
}

void UTankAimingComponent::AddAmmo(int32 Amount)
{
	RoundsLeft = RoundsLeft + Amount;
	RoundsLeft = FMath::Clamp<int32>(RoundsLeft, 0, MaxRounds);
}

void UTankAimingComponent::IncreaseLaunchSpeed()
{
	if (LaunchSpeed <= (MaxLaunchSpeed - 100))
	{
		LaunchSpeed = LaunchSpeed + 100;
	}
}

void UTankAimingComponent::DecreaseLaunchSpeed()
{
	if (LaunchSpeed >= (MinLaunchSpeed + 100))
	{
		LaunchSpeed = LaunchSpeed - 100;
	}
}

void UTankAimingComponent::Disable(float Time)
{
	bAimingDisabled = true;

	FTimerHandle DisableTimer;
	GetWorld()->GetTimerManager().SetTimer(DisableTimer, this, &UTankAimingComponent::OnEnable, Time, false);
}

void UTankAimingComponent::OnEnable()
{
	bAimingDisabled = false;
}