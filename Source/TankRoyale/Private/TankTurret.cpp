// Copyright Blurr Development 2018.

#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	// Move the barrel the right amount this frame
	// Given a max rotation speed, and the frame time.
	if (!bIsDamaged && !bIsTotalled)
	{
		// Rotate normally
		RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
		auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
		auto Rotation = RelativeRotation.Yaw + RotationChange;
		SetRelativeRotation(FRotator(0, Rotation, 0));
	}
	else if (!bIsTotalled && bIsDamaged)
	{
		// Slower rotation as it is damaged
		RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
		auto RotationChange = RelativeSpeed * MaxDegreesPerSecondDamaged * GetWorld()->DeltaTimeSeconds;
		auto Rotation = RelativeRotation.Yaw + RotationChange;
		SetRelativeRotation(FRotator(0, Rotation, 0));
	}
	else if (bIsTotalled)
	{
		// To damaged to rotate.
	}
}

float UTankTurret::TakeDamage(float DamageAmount)
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
	}

	return DamageToApply;
}

// GETTERS AND SETTERS
float UTankTurret::GetTurretDamage() const
{
	return ((float)CurrentDamage / (float)StartingDamage);
}

bool UTankTurret::IsTurretDamaged() const
{
	return bIsDamaged;
}

bool UTankTurret::IsTurretTotalled() const
{
	return bIsTotalled;
}