// Copyright Blurr Development 2018.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "TankPlayerController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "tank.h"
const FName Enemy = "Enemy";
ATankAIController::ATankAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATankAIController::OnTargetPerceptionUpdated);
	RunBehaviorTree(TankBehaviorTree);
	UseBlackboard(TankBlackboardData, BlackboardComponent);
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		 PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::OnTargetPerceptionUpdated(AActor * Actor, FAIStimulus AIStimulus)
{
	// Check actor team
	if (Actor->ActorHasTag(PossessedTank->TeamOneTag))
	{
		// True means that they are on the same team and we sould not do anything
		if (PossessedTank->ActorHasTag(PossessedTank->TeamOneTag)) {

		}
		// True means that they are not on the same team and we sould fuck him up
		else if ((PossessedTank->ActorHasTag(PossessedTank->TeamTwoTag)))
		{
			BlackboardComponent->SetValueAsObject(Enemy, Actor);
		}
		
	}
}
void ATankAIController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	/*
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();
	if (!(PlayerTank && ControlledTank)) { return; }

	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius); // TODO Make sure radius is in cm for Acceptance Radius
		
	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation() + 150);

	// FIRE!
	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire();
	}
	*/
}