// Copyright Blurr Development 2018.

#include "GadgetMine.h"

AGadgetMine::AGadgetMine()
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionSphere->SetSphereRadius(500);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGadgetMine::OnOverlapBegin);
}

// When another actor overlaps the collision mesh.
void AGadgetMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("USE MINE"));
}

// Override use gadget.
void AGadgetMine::UseGadget()
{
	Super::UseGadget();
}
