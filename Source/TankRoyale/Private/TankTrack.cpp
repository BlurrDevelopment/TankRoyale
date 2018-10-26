// Copyright Blurr Development 2018.

#include "TankTrack.h"
#include "Engine/World.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"
#include "TankMovementComponent.h"

UTankTrack::UTankTrack()
{
	// Allows the component to be able to tick.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
}
void UTankTrack::SetThrottle(FMove Move, bool bIsLeft)
{
	if (!bIsLeft)
	{
		float CurrentThrottle = FMath::Clamp<float>(Move.RightThrottle, -1.0f, +1.0f);
		DriveTrack(CurrentThrottle, Move);
	}
	if (bIsLeft)
	{
		float CurrentThrottle = FMath::Clamp<float>(Move.LeftThrottle, -1.0f, +1.0f);
		DriveTrack(CurrentThrottle, Move);
	}


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

void UTankTrack::DriveTrack(float CurrentThrottle,  FMove Move)
{
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce ;
	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel * Move.DeltaTime);
	}
}


