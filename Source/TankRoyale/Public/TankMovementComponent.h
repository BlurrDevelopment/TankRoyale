// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"
USTRUCT()
struct FMove {

	GENERATED_USTRUCT_BODY()
		UPROPERTY()
		float LeftThrottle;
	UPROPERTY()
		float RightThrottle;
	UPROPERTY()
		float DeltaTime;
	UPROPERTY()
		float Time;
};
// Forward declaration
class UTankTrack;

// Responsible for driving the tanks tracks
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKROYALE_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet);
	UTankMovementComponent();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void IntendTurnRight(float Throw);
	float LeftThrottle;
	float RightThrottle;
	/// Disable Movement
	void Disable(float Time);
	void OnEnable();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	// Called from the path finding logic by the AI Controllers
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;

	/// Disable Movement
	bool bMovementDisabled = false;

	FMove MakeMove(float DeltaTime);
	FMove Move;
};
