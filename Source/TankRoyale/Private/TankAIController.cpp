// Copyright Blurr Development 2018.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankPlayerController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Kismet/GameplayStatics.h"
#include "tank.h"
const FName Enemy = "Enemy";
const FName LastSeenLocation = "LastSeenLocation";
const FName PickUp = "PickUp";

ATankAIController::ATankAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSense"));
	HearingSenseConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearningSense"));
	SenseConfigDamage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("SenseDamage"));
	SightSenseConfig->SightRadius = 80000.f;
	SightSenseConfig->LoseSightRadius = 10000.f;
	SightSenseConfig->PeripheralVisionAngleDegrees = 180.f;
	SightSenseConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingSenseConfig->HearingRange = 12500.f;
	HearingSenseConfig->DetectionByAffiliation.bDetectNeutrals = false;
	HearingSenseConfig->DetectionByAffiliation.bDetectEnemies = true;
	AIPerceptionComponent->ConfigureSense(*Cast<UAISenseConfig>(SightSenseConfig));
	AIPerceptionComponent->ConfigureSense(*Cast<UAISenseConfig>(HearingSenseConfig));
	AIPerceptionComponent->ConfigureSense(*Cast<UAISenseConfig>(SenseConfigDamage));
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
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	
	//GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::OnTargetPerceptionUpdated(AActor * Actor, FAIStimulus AIStimulus)
{
	if (PossessedTank->GetbGameStarted())
	{
		UE_LOG(LogTemp, Warning, TEXT(" tank team %d"), PossessedTank->GetTankTeam());
		if (Actor->ActorHasTag(PickUp))
		{
			BlackboardComponent->SetValueAsObject(PickUp, Actor);
		}
		// not the best way but good enough for now  
		ATank * TankActor = Cast<ATank>(Actor);
		if (TankActor != nullptr)
		{
			if (TankActor->GetTankTeam() != PossessedTank->GetTankTeam())
			{
				UE_LOG(LogTemp, Warning, TEXT(" pynohn "))
				BlackboardComponent->SetValueAsObject(Enemy, Actor);
				BlackboardComponent->SetValueAsVector(LastSeenLocation, Actor->GetActorLocation());
				return;
			}
		}
		BlackboardComponent->SetValueAsObject(Enemy, nullptr);
	}

}



void ATankAIController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}