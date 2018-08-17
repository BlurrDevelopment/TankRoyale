// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Projectile.generated.h"

class ATank;

UCLASS()
class TANKROYALE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchProjectile(float Speed);

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ImpactSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float VolumeMultiplier = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float StartTime = 0.0f;

	void SetFiringTank(ATank* Tank);

private:
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	void OnTimerExpire();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DestroyDelay = 5.0f;

	//UPROPERTY(EditDefaultsOnly, Category = "Setup")
	//float ProjectileDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileMaxDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ProjectileMinDamage = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float BlastRadius = 200.0f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* CollisionMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* LaunchBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ImpactBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* ExplosionForce = nullptr;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	ATank* OwningTank;
};
