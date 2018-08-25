// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Gadgets/TankGadget.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Engine/World.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GadgetMine.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API AGadgetMine : public ATankGadget
{
	GENERATED_BODY()
	
public:
	// Called when the game starts or when spawned.
	AGadgetMine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// When another actor overlaps the collision mesh.
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Override use gadget.
	virtual void UseGadget() override;

	// Make the mine explode.
	void ExplodeMine();

	// Collision Sphere for Overlap.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionSphere = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* ExplosionForce = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* MineBlast = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ExplodeSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MineMaxDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MineMinDamage = 30.0f;
};
