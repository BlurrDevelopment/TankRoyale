// Copyright Blurr Development 2018.

#include "TankMovementComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "TankTrack.h"

UTankMovementComponent::UTankMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	if (!ensure(LeftTrackToSet && RightTrackToSet)) { return; }
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
	
}
void UTankMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Move = MakeMove(DeltaTime);
	//if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy || GetOwnerRole() == ROLE_AutonomousProxy) {
	RightTrack->SetThrottle(Move, false);
		LeftTrack->SetThrottle(Move, true);
	
	//}
}


void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (bMovementDisabled) return;
	if (LeftThrottle == 0)
	{
		LeftThrottle = Throw;
	}
	if (RightThrottle == 0 ) {
		RightThrottle = Throw;
	}
	
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (bMovementDisabled) return;
	LeftThrottle = Throw;
	RightThrottle = -Throw;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call super as replacing functionality
	if (bMovementDisabled) return;

	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention);

	IntendMoveForward(ForwardThrow);
	IntendTurnRight(RightThrow.Z);
	
	// auto TankName = GetOwner()->GetName();
	// auto Time = GetWorld()->GetTimeSeconds();
	// UE_LOG(LogTemp, Warning, TEXT("%f: %s direct move at %s"), Time, *TankName, *MoveVelocityString);

}

void UTankMovementComponent::Disable(float Time)
{
	bMovementDisabled = true;

	FTimerHandle DisableTimer;
	GetWorld()->GetTimerManager().SetTimer(DisableTimer, this, &UTankMovementComponent::OnEnable, Time, false);
}

void UTankMovementComponent::OnEnable()
{
	bMovementDisabled = false;
}
FMove UTankMovementComponent::MakeMove(float DeltaTime)
{
	FMove Move;
	Move.DeltaTime = DeltaTime;
	Move.Time = UGameplayStatics::GetGameState(GetWorld())->GetServerWorldTimeSeconds();
	Move.LeftThrottle = LeftThrottle;
	Move.RightThrottle = RightThrottle;
	return Move;
}
