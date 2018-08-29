// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TankGadget.generated.h"

class ATank;

UCLASS()
class TANKROYALE_API ATankGadget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	ATankGadget();

	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Fire the gadget.
	void LaunchGadget(float Speed);

	// Destroy the gadget.
	void DestroyGadget();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Control the hit event.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Stick to the surface hit.
	void StickToSurface(AActor* Surface, FTransform MineTransform);

	// Activate the gadget.
	virtual void ActivateGadget();

	// Deactivate the gadget.
	virtual void DeactivateGadget();

	// Use the gadget.
	virtual void UseGadget();

	/// Vars

	// The tank that owns the gadget.
	ATank* OwningTank;

	// The surface the gadget is stuck to.
	AActor* SurfaceStuckTo;

	// If the gadget has been launched.
	bool bLaunchedGadget = false;

	// If the gadget is active.
	bool bActiveGadget = false;
	
	// Movement component
	UProjectileMovementComponent* GadgetMovement = nullptr;

	// The collision mesh for the gadget
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* CollisionMesh = nullptr;

};
