// Copyright Blurr Development 2018.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	// Allows the component to be able to tick.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
}
void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1.0f, +1.0f);
	DriveTrack(CurrentThrottle);
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> ResultArray;

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	for (USceneComponent* Child : Children)
	{
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;

		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel) continue;

		ResultArray.Add(SprungWheel);
	}

	return ResultArray;
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}


