// Copyright Blurr Development 2018.

#include "Tank.h"
#include "Engine/World.h"
#include "GameModes/GameModeDeathmatch.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//DeathBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Death Blast"));
	//DeathBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//DeathBlast->bAutoActivate = false;
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
		DeathBlast->Activate();
		bDead = true;

		OnDeath.Broadcast();

		if (GameMode == EGameMode::Deathmatch)
		{
			auto KillerTank = Cast<ATank>(DamageCauser);
			Cast<AGameModeDeathmatch>(GetWorld()->GetAuthGameMode())->AddTeamDeath(this, KillerTank);
		}

		// Play explosion sound
		if (!ensure(ExplodeSound)) return DamageToApply;
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation(), ExplodeVolumeMultiplier, ExplodePitchMultiplier, ExplodeStartTime);
	}

	return DamageToApply;
}

float ATank::GetHealthPercent() const
{
	return ((float)CurrentHealth / (float)StartingHealth);
}