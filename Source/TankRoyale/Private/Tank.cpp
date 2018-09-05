// Copyright Blurr Development 2018.

#include "Tank.h"
#include "TankTurret.h"
#include "TankTrack.h"
#include "Engine/World.h"
#include "GameModes/GameModeDeathmatch.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup.h"
#include "TankAimingComponent.h"


// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = StartingHealth;

	if (Cast<AGameModeDeathmatch>(GetWorld()->GetAuthGameMode())) GameMode = EGameMode::Deathmatch;

	if (GameMode == EGameMode::Deathmatch)
	{
		Cast<AGameModeDeathmatch>(GetWorld()->GetAuthGameMode())->AssignTankTeam(this);
	}
}


float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;

	// Play tank damage sound
	if (!ensure(DamageSound)) return DamageToApply;
	UGameplayStatics::PlaySoundAtLocation(this, DamageSound, GetActorLocation(), DamageVolumeMultiplier, DamagePitchMultiplier, DamageStartTime);

	if (!bDead)
	{
		if (CurrentHealth <= (StartingHealth / 2) && CurrentHealth > 0)
		{
			// Play the particle emitter
			if (!ensure(SmokeEmitterTemplate)) return DamageToApply;
			SmokeEmitterComponent = UGameplayStatics::SpawnEmitterAttached(SmokeEmitterTemplate, FindComponentByClass<UTankTurret>(), FName("Centre"), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), EAttachLocation::KeepRelativeOffset, true);
		}
		else if (CurrentHealth <= 0)
		{
			if (SmokeEmitterComponent) SmokeEmitterComponent; // TODO Destroy SmokeEmitterComponent
			TankDeath(DamageCauser, DamageToApply);
		}
		else
		{
			if (SmokeEmitterComponent) SmokeEmitterComponent; // TODO Destroy SmokeEmitterComponent
		}
	}

	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return ((float)CurrentHealth / (float)StartingHealth);
}

void ATank::SetOnPickup(bool On, APickup* Pickup)
{
	bOnPickup = On;
	CurrentPickup = Pickup;
}

void ATank::TankDeath(AActor* DamageCauser, int32 DamageToApply)
{
	bDead = true;
	OnDeath.Broadcast();

	if (GameMode == EGameMode::Deathmatch)
	{
		auto KillerTank = Cast<ATank>(DamageCauser);
		Cast<AGameModeDeathmatch>(GetWorld()->GetAuthGameMode())->AddTeamDeath(this, KillerTank);

		// Drop all remaining ammo
		DropRemainingAmmo();

		// 40% Chance to drop health
		int32 Chance = rand() % 100 + 1;
		if (Chance <= 40) DropHealth(50);

		// 15% Chance to drop burst
		Chance = rand() % 100 + 1;
		if (Chance <= 20) DropBurst();
	}

	// Play explosion sound
	if (!ensure(ExplodeSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation(), ExplodeVolumeMultiplier, ExplodePitchMultiplier, ExplodeStartTime);

	// Play the particle emitter
	if (!ensure(DeathEmitterTemplate)) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathEmitterTemplate, GetTransform());

	// Destroy the actor
	Destroy();

	return;
}

void ATank::Repair(float Amount)
{
	Amount = FMath::Clamp<float>(Amount, 0.0f, 100.0f);
	CurrentHealth = FMath::Clamp<int32>(CurrentHealth + Amount, 0, 100);
	UE_LOG(LogTemp, Warning, TEXT("%s: I have been repaired for: %f"), *GetName(), Amount);
}

void ATank::DropRemainingAmmo()
{
	if (!ensure(AmmoPickupBlueprint)) return;
	FVector Location = GetActorLocation() + AmmoOffset;
	auto SpawnedPickup = GetWorld()->SpawnActor<APickup>(AmmoPickupBlueprint);
	SpawnedPickup->SetActorLocation(Location);

	auto AimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	int32 Ammo = AimingComponent->GetRoundsLeft();
	if (Ammo <= 0) return;
	SpawnedPickup->SetupPickup(EPickupType::Ammo, Ammo);
}

void ATank::DropHalfAmmo()
{
	if (!ensure(AmmoPickupBlueprint)) return;
	FVector Location = GetActorLocation() + AmmoOffset;
	auto SpawnedPickup = GetWorld()->SpawnActor<APickup>(AmmoPickupBlueprint);
	SpawnedPickup->SetActorLocation(Location);

	auto AimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	int32 Ammo = (AimingComponent->GetRoundsLeft()) / 2;
	if (Ammo <= 0) return;
	SpawnedPickup->SetupPickup(EPickupType::Ammo, Ammo);
}

void ATank::DropAmmo(int32 Amount)
{
	if (Amount <= 0) return;
	if (!ensure(AmmoPickupBlueprint)) return;
	FVector Location = GetActorLocation() + AmmoOffset;
	auto SpawnedPickup = GetWorld()->SpawnActor<APickup>(AmmoPickupBlueprint);
	SpawnedPickup->SetActorLocation(Location);
	SpawnedPickup->SetupPickup(EPickupType::Ammo, Amount);
}

void ATank::DropHealth(int32 Amount)
{
	if (Amount <= 0) return;
	if (!ensure(HealthPickupBlueprint)) return;
	FVector Location = GetActorLocation() + HealthOffset;
	auto SpawnedPickup = GetWorld()->SpawnActor<APickup>(HealthPickupBlueprint);
	SpawnedPickup->SetActorLocation(Location);
	SpawnedPickup->SetupPickup(EPickupType::Health, Amount);
}

void ATank::DropBurst()
{
	if (!ensure(BurstPickupBlueprint)) return;
	FVector Location = GetActorLocation() + BurstOffset;
	auto SpawnedPickup = GetWorld()->SpawnActor<APickup>(BurstPickupBlueprint);
	SpawnedPickup->SetActorLocation(Location);
	SpawnedPickup->SetupPickup(EPickupType::Burst, 0);
}

void ATank::UsePickup()
{
	if (CurrentPickup->GetType() == EPickupType::Ammo) UseAmmoPickup();
	if (CurrentPickup->GetType() == EPickupType::Health) UseHealthPickup();
	if (CurrentPickup->GetType() == EPickupType::Burst) UseBurstPickup();
}

void ATank::UseAmmoPickup()
{
	// Get the aiming component and then add the ammo from the pickup
	auto AimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;

	int32 AmountToPickup = CurrentPickup->GetValue();
	int32 AmountToLeave = 0;

	// Make sure the tank isn't already full of ammo
	if (AimingComponent->GetRoundsLeft() >= AimingComponent->GetMaxRounds()) return;

	// Check how much the tank can take and set AmountToLeave to what is left
	if ((AimingComponent->GetMaxRounds() - AimingComponent->GetRoundsLeft()) < AmountToPickup)
	{
		AmountToLeave = AmountToPickup;
		AmountToPickup = (AimingComponent->GetMaxRounds() - AimingComponent->GetRoundsLeft());
		AmountToLeave = AmountToLeave - AmountToPickup;
	}

	// Add the ammo to the tank
	AimingComponent->AddAmmo(AmountToPickup);

	// Destroy the pickup
	CurrentPickup->Deactivate();

	// Spawn new pickup with what is left
	if (AmountToLeave > 0) DropAmmo(AmountToLeave);
}

void ATank::UseHealthPickup()
{
	int32 AmountToPickup = CurrentPickup->GetValue();
	int32 AmountToLeave = 0;

	// Make sure the tank isn't already full health
	if (CurrentHealth >= StartingHealth) return;

	// Check how much the tank can take and set AmountToLeave to what is left
	if ((StartingHealth - CurrentHealth) < AmountToPickup)
	{
		AmountToLeave = AmountToPickup;
		AmountToPickup = (StartingHealth - CurrentHealth);
		AmountToLeave = AmountToLeave - AmountToPickup;
	}

	// Add the health
	CurrentHealth = CurrentHealth + AmountToPickup;

	// Destroy
	CurrentPickup->Deactivate();

	// Spawn new pickup with what is left
	if (AmountToLeave > 0) DropHealth(AmountToLeave);
}

void ATank::UseBurstPickup()
{
	// Get the aiming component and then add the ammo from the pickup
	auto AimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;

	// Use the pickup
	AimingComponent->LoadSpecialAmmo();

	// Destroy the pickup
	CurrentPickup->Deactivate();
}