// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

//Forward Declarations
class UTankAimingComponent;
class UTank;

// Responsible for helping the player control the tank.
UCLASS()
class TANKROYALE_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Override BeginPlay.
	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

	// Override Tick.
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

private:
	UPROPERTY(EditDefaultsOnly)
	float CrosshairXLocation = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairYLocation = 0.33333f;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0f; // 10km

	// Move the barrel towards the aimpoint/crosshair.
	void AimTowardsCrosshair();
	
	// Return an OUT parameter, true if hit landscape.
	bool GetSightRayHitLocation(FVector& HitLocation) const;

	// Get where the player is looking at in the world.
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	// Get the direction the player is looking in.
	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	UFUNCTION()
	void OnPossessedTankDeath();
};
