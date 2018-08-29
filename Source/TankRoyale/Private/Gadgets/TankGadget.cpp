// Copyright Blurr Development 2018.

#include "TankGadget.h"


// Sets default values
ATankGadget::ATankGadget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(true);

	GadgetMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Gadget Movement"));
	GadgetMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATankGadget::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &ATankGadget::OnHit);
}

// Called every frame
void ATankGadget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATankGadget::LaunchGadget(float Speed)
{
	bLaunchedGadget = true;
	GadgetMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	GadgetMovement->Activate();
}

void ATankGadget::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bActiveGadget)
	{
		StickToSurface(OtherActor, this->GetActorTransform());
		ActivateGadget();
	}
}

void ATankGadget::StickToSurface(AActor* Surface, FTransform MineTransform)
{
	GadgetMovement->StopMovementImmediately();
	GadgetMovement->Deactivate();
	GadgetMovement->DestroyComponent();
	SetActorTransform(MineTransform);
	SurfaceStuckTo = Surface;
}

void ATankGadget::ActivateGadget()
{
	bActiveGadget = true;
}

void ATankGadget::DeactivateGadget()
{
	bActiveGadget = false;
}

void ATankGadget::UseGadget()
{
	DeactivateGadget();
}

void ATankGadget::DestroyGadget()
{
	Destroy();
}