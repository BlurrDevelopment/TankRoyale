// Copyright Blurr Development 2018.

#include "Pickup.h"
#include "Tank.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("Pickup Particle"));
	SetRootComponent(PickupParticle);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->AttachToComponent(PickupParticle, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionSphere->SetSphereRadius(500);
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("AIPerceptionStimuliSourceComponent"));
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APickup::OnOverlapEnd);

}

void APickup::SetupPickup(EPickupType Type, int32 Value)
{
	PickupType = Type;
	PickupValue = Value;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("PickUp");
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::Deactivate()
{
	PickupParticle->DestroyComponent();
	Destroy();
}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATank>(OtherActor))
	{
		Cast<ATank>(OtherActor)->SetOnPickup(true, this);
	}
}

void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ATank>(OtherActor))
	{
		Cast<ATank>(OtherActor)->SetOnPickup(false, this);
	}
}