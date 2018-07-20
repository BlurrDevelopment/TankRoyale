// Copyright Blurr Development 2018.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetPawn();
	if (!ControlledTank)
	{
		// TODO Display error on screen. Code: b134a3ba-89e8-11e8-9a94-a6cf71072f73
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
		UE_LOG(LogTemp, Error, TEXT("    Please contact support with the error code."));
		UE_LOG(LogTemp, Error, TEXT("    Discord Support: https://discord.gg/yv3Y25f"));
		UE_LOG(LogTemp, Error, TEXT(" ERROR CODE: b134a3ba-89e8-11e8-9a94-a6cf71072f73"));
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
	}

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (AimingComponent)
	{
		FoundAimingComponent(AimingComponent);
	}
	else
	{
		// TODO Display error on screen. Code: b1348c86-89e8-11e8-9a94-a6cf71072f73
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
		UE_LOG(LogTemp, Error, TEXT("    Please contact support with the error code."));
		UE_LOG(LogTemp, Error, TEXT("    Discord Support: https://discord.gg/yv3Y25f"));
		UE_LOG(LogTemp, Error, TEXT(" ERROR CODE: b1348c86-89e8-11e8-9a94-a6cf71072f73"));
		UE_LOG(LogTemp, Error, TEXT("------------------------------------------------------"));
	}
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } // e.g. if not possessing

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // Out parameter
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);

	if (bGotHitLocation) // Has "side-effect", is going to line trace.
	{
		AimingComponent->AimAt(HitLocation);
	}
}

// Get if the aimpoint is aiming at the landscape.
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	// Find the crosshair position
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(OUT ViewportSizeX, OUT ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	// "De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, OUT LookDirection))
	{
		// Line-trace along that LookDirection, and see waht we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	// If linetrace succeeds set the HitLocation
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0.0f);
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be disgarded

	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}
