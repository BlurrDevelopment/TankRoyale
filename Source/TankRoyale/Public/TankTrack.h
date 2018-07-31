// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

class ASprungWheel;

/*
* TankTrack is to make the tank drivable by the player.
*/
UCLASS(meta = (BlueprintSpawnableComponent))//, hidecategories = ("Collision"))
class TANKROYALE_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// Sets the throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetThrottle(float Throttle);
	
	// Max force per track, in Newtons
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TrackMaxDrivingForce = 400000; // Assume 40 tonne tank, and 1g acceleration 

private:
	UTankTrack();

	void BeginPlay();

	TArray<ASprungWheel*> GetWheels() const;

	void DriveTrack(float CurrentThrottle);
};
