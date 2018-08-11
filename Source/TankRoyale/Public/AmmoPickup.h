// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "AmmoPickup.generated.h"

UCLASS()
class TANKROYALE_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Setup the pickup
	void SetupPickup(int32 Ammo);

	// Deactivate and destroy the pickup
	void Deactivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	int32 AmmoStored = 5;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* PickupParticle = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionSphere = nullptr;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
