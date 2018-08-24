// Copyright Blurr Development 2018.

#include "GadgetMine.h"

AGadgetMine::AGadgetMine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionSphere->SetSphereRadius(500);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGadgetMine::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGadgetMine::BeginPlay()
{
	Super::BeginPlay();
}

// When another actor overlaps the collision mesh.
void AGadgetMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActiveGadget) UseGadget();
}

// Override use gadget.
void AGadgetMine::UseGadget()
{
	Super::UseGadget();
}
