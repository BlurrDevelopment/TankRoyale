// Copyright Blurr Development 2018.

#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "GameModeDeathmatch.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(true);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{	// Deactivate launch blast and activate the impact blast and fire the explosion force
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();

	// Set the impact blast to the root and destroy the collision mesh
	SetRootComponent(ImpactBlast);
	CollisionMesh->DestroyComponent();

	// Apply the damage from the projectile
	float ProjectileDamage = FMath::RandRange(ProjectileMinDamage, ProjectileMaxDamage);
	UGameplayStatics::ApplyRadialDamage(this, ProjectileDamage, GetActorLocation(), ExplosionForce->Radius, UDamageType::StaticClass(), TArray<AActor*>());

	// Start the timer for destroying the projectile
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectile::OnTimerExpire, DestroyDelay, false);

	// Play explosion sound
	if (!ensure(ImpactSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), VolumeMultiplier, PitchMultiplier, StartTime);

	// TODO Get what the projectile hit
	// Check if the projectile hit a tank.
	auto HitTank = Cast<ATank>(OtherActor);
	if (HitTank)
	{
		// TODO Send to gamemode the owning tank and hit tank
		auto GM = Cast<AGameModeDeathmatch>(GetWorld()->GetAuthGameMode());
		if (OwningTank && GM)
		{
			GM->RegisterTankHit(OwningTank, HitTank);
		}
	}
}


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::LaunchProjectile(float Speed)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	ProjectileMovement->Activate();
}

void AProjectile::OnTimerExpire()
{
	Destroy();
}

void AProjectile::SetFiringTank(ATank* Tank)
{
	if (!ensure(Tank)) return;
	OwningTank = Tank;
}