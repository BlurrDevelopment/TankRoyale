// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Gadgets/TankGadget.h"
#include "Components/SphereComponent.h"
#include "GadgetMine.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API AGadgetMine : public ATankGadget
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned.
	AGadgetMine();

private:
	// Activate the mine so it blows when driven over.
	virtual void ActivateGadget() override;

	// Deactivate The mine so it will no longer blow when driven over.
	void DeactivateMine();

	// When another actor overlaps the collision mesh.
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Override use gadget.
	virtual void UseGadget() override;

	// Collision Sphere for Overlap.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionSphere = nullptr;
};
