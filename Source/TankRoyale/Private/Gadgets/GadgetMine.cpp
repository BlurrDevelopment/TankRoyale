// Copyright Blurr Development 2018.

#include "GadgetMine.h"
#include "Kismet/GameplayStatics.h"

AGadgetMine::AGadgetMine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionSphere->SetSphereRadius(500);

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGadgetMine::OnOverlapBegin);

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionForce->Radius = 500;

	MineBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Mine Blast"));
	MineBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MineBlast->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AGadgetMine::BeginPlay()
{
	Super::BeginPlay();
}

// When another actor overlaps the collision mesh.
void AGadgetMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActiveGadget)
	{
		UseGadget();
	}
}

// Override use gadget.
void AGadgetMine::UseGadget()
{
	Super::UseGadget();
	ExplodeMine();
}

void AGadgetMine::ExplodeMine()
{
	// Activate the blast and fire the explosion force
	MineBlast->Activate();
	ExplosionForce->FireImpulse();

	// Set the blast as root and destroy any collision meshes
	SetRootComponent(MineBlast);
	CollisionMesh->DestroyComponent();
	CollisionSphere->DestroyComponent();

	// Play the explode sound
	if (!ensure(ExplodeSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation(), 1.0f, 1.0f, 0.0f);

	// Apply damage to the explosion range
	float MineDamage = FMath::RandRange(MineMinDamage, MineMaxDamage);
	UGameplayStatics::ApplyRadialDamage(this, MineDamage, GetActorLocation(), ExplosionForce->Radius, UDamageType::StaticClass(), TArray<AActor*>());
}