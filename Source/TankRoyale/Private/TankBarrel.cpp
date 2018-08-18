// Copyright Blurr Development 2018.

#include "TankBarrel.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	if (bOverheated) return;

	// Move the barrel the right amount this frame
	// Given a max elevation speed, and the frame time.
	if (!bIsDamaged)
	{
		// Normal elavation
		RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
		auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
		auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
		auto Elevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
		SetRelativeRotation(FRotator(Elevation, 0, 0));
	}
	else
	{
		// Slower elavation as it is damaged
		RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
		auto ElevationChange = RelativeSpeed * MaxDegreesPerSecondDamaged * GetWorld()->DeltaTimeSeconds;
		auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
		auto Elevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
		SetRelativeRotation(FRotator(Elevation, 0, 0));
	}
}

float UTankBarrel::TakeDamage(float DamageAmount)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp<int32>(DamagePoints, 0, CurrentDamage);

	CurrentDamage = CurrentDamage - DamageToApply;

	if (CurrentDamage <= StartDamagedEffects)
	{
		bIsDamaged = true;
	}

	if (CurrentDamage <= TotalPart)
	{
		bIsTotalled = true;
		bCanFire = false;
	}
	else
	{
		bCanFire = true;
	}

	return DamageToApply;
}

float UTankBarrel::GetBarrelDamage() const
{
	return ((float)CurrentDamage / (float)StartingDamage);
}

bool UTankBarrel::IsBarrelDamaged() const
{
	return bIsDamaged;
}

bool UTankBarrel::IsBarrelTotalled() const
{
	return bIsTotalled;
}

bool UTankBarrel::CanBarrelFire() const
{
	return bCanFire;
}

