// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Pickup.generated.h"

UENUM()
enum class EPickupType : uint8
{
	Ammo,
	Health,
	Burst
};

UCLASS()
class TANKROYALE_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Setup the pickup
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetupPickup(EPickupType Type, int32 Value);

	// Deactivate and destroy the pickup
	UFUNCTION(BlueprintCallable, Category = "Use")
	void Deactivate();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	int32 GetValue() { return PickupValue; }
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	EPickupType GetType() { return PickupType; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UAIPerceptionStimuliSourceComponent * AIPerceptionStimuliSourceComponent;
private:
	int32 PickupValue = 5;
	EPickupType PickupType = EPickupType::Ammo;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* PickupParticle = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionSphere = nullptr;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
