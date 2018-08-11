// Copyright Blurr Development 2018.

#include "Tank.h"
#include "Engine/World.h"
#include "GameModes/GameModeDeathmatch.h"
#include "Kismet/GameplayStatics.h"
#include "AmmoPickup.h"
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

	if (CurrentHealth <= 0 && bDead == false)
	{
		TankDeath(DamageCauser, DamageToApply);
	}

	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return ((float)CurrentHealth / (float)StartingHealth);
}

void ATank::SetOnPickup(bool On, AAmmoPickup* Pickup)
{
	bOnPickup = On;
	CurrentPickup = Pickup;
}

int32 ATank::TankDeath(AActor* DamageCauser, int32 DamageToApply)
{
	bDead = true;

	OnDeath.Broadcast();

	if (GameMode == EGameMode::Deathmatch)
	{
		auto KillerTank = Cast<ATank>(DamageCauser);
		Cast<AGameModeDeathmatch>(GetWorld()->GetAuthGameMode())->AddTeamDeath(this, KillerTank);

		DropRemainingAmmo();
	}

	// Play explosion sound
	if (!ensure(ExplodeSound)) return DamageToApply;
	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation(), ExplodeVolumeMultiplier, ExplodePitchMultiplier, ExplodeStartTime);
	
	return DamageToApply;
}

void ATank::DropRemainingAmmo()
{
	if (!ensure(AmmoPickupBlueprint)) return;
	FVector Location = GetActorLocation();
	auto SpawnedPickup = GetWorld()->SpawnActor<AAmmoPickup>(AmmoPickupBlueprint);
	SpawnedPickup->SetActorLocation(Location);

	auto AimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	int32 Ammo = AimingComponent->GetRoundsLeft();
	SpawnedPickup->SetupPickup(Ammo);
}

void ATank::DropHalfAmmo()
{
	if (!ensure(AmmoPickupBlueprint)) return;
	FVector Location = GetActorLocation();
	auto SpawnedPickup = GetWorld()->SpawnActor<AAmmoPickup>(AmmoPickupBlueprint);
	SpawnedPickup->SetActorLocation(Location);

	auto AimingComponent = FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) return;
	int32 Ammo = (AimingComponent->GetRoundsLeft())/2;
	SpawnedPickup->SetupPickup(Ammo);
}

void ATank::UsePickup()
{
	// Get the aiming component and then add the ammo from the pickup
	auto AimingComponent = FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AddAmmo(CurrentPickup->GetStoredAmmo());

	// Destroy the pickup
	CurrentPickup->Deactivate();
	UE_LOG(LogTemp, Warning, TEXT("Use Pickup."))
}