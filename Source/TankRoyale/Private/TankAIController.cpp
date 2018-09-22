// Copyright Blurr Development 2018.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "TankPlayerController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig.h"
#include "tank.h"
const FName Enemy = "Enemy";
ATankAIController::ATankAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSense"));
	HearingSenseConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearningSense"));
	SightSenseConfig->SightRadius = 80000.f;
	SightSenseConfig->LoseSightRadius = 10000.f;
	SightSenseConfig->PeripheralVisionAngleDegrees = 180.f;
	SightSenseConfig->DetectionByAffiliation.DetectAllFlags();
	HearingSenseConfig->HearingRange = 12500.f;
	HearingSenseConfig->DetectionByAffiliation.DetectAllFlags();
	AIPerceptionComponent->ConfigureSense(*Cast<UAISenseConfig>(SightSenseConfig));
	AIPerceptionComponent->ConfigureSense(*Cast<UAISenseConfig>(HearingSenseConfig));
	
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATankAIController::OnTargetPerceptionUpdated);
	UseBlackboard(TankBlackboardData, BlackboardComponent);
	RunBehaviorTree(TankBehaviorTree);
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		 PossessedTank = Cast<ATank>(InPawn);
		 // TODO for testing
	//	if (!ensure(PossessedTank)) { return; }
	//	PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::OnTargetPerceptionUpdated(AActor * Actor, FAIStimulus AIStimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionUpdated"));
	/*
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
	else if (Actor->ActorHasTag(PossessedTank->TeamTwoTag)) {
		// True means that they are on the same team and we sould not do anything
		if (PossessedTank->ActorHasTag(PossessedTank->TeamTwoTag)) {

		}
		// True means that they are not on the same team and we sould fuck him up
		else if ((PossessedTank->ActorHasTag(PossessedTank->TeamOneTag)))
		{
			BlackboardComponent->SetValueAsObject(Enemy, Actor);
		}

	}
	*/
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