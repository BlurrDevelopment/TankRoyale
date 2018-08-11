// Copyright Blurr Development 2018.

#include "AmmoPickup.h"


// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("Pickup Particle"));
	PickupParticle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->AttachToComponent(PickupParticle, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AAmmoPickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAmmoPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoPickup::SetupPickup(int32 Ammo)
{
	AmmoStored = Ammo;
}

void AAmmoPickup::Deactivate()
{
	// Destroy ui component
	PickupParticle->Deactivate();
	Destroy();
}
