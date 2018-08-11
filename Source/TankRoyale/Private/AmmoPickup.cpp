// Copyright Blurr Development 2018.

#include "AmmoPickup.h"
#include "Tank.h"


// Sets default values
AAmmoPickup::AAmmoPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("Pickup Particle"));
	PickupParticle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->AttachToComponent(PickupParticle, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionSphere->SetSphereRadius(1250);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPickup::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AAmmoPickup::OnOverlapEnd);

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
	PickupParticle->DestroyComponent();
	Destroy();
}

void AAmmoPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATank>(OtherActor))
	{
		Cast<ATank>(OtherActor)->SetOnPickup(true, this);
	}
}

void AAmmoPickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ATank>(OtherActor))
	{
		Cast<ATank>(OtherActor)->SetOnPickup(false, this);
	}
}