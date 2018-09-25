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
#include "tank.h"
const FName Enemy = "Enemy";
const FName LastSeenLocation = "LastSeenLocation";
const FName TeamOneTag = "1";
const FName TeamTwoTag = "2";
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
	SightSenseConfig->DetectionByAffiliation.DetectAllFlags();
	HearingSenseConfig->HearingRange = 12500.f;
	HearingSenseConfig->DetectionByAffiliation.DetectAllFlags();
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
		 // TODO for testing
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

	UE_LOG(LogTemp, Warning, TEXT("OnTargetPerceptionUpdated"));
	if (Actor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());

			UE_LOG(LogTemp, Warning, TEXT("SensingSucceeded + Actor != nullptr "))
				if (Actor->ActorHasTag(PickUp))
				{
					BlackboardComponent->SetValueAsObject(PickUp, Actor);
					UE_LOG(LogTemp, Warning, TEXT("PickUp"));
				}
				// Check actor team
				if (Actor->ActorHasTag(TeamOneTag))
				{
					// True means that they are not on the same team and we should fuck him up
					if ((PossessedTank->ActorHasTag(TeamTwoTag)))
					{
						BlackboardComponent->SetValueAsObject(Enemy, Actor);
						BlackboardComponent->SetValueAsVector(LastSeenLocation, Actor->GetActorLocation());
						
					}

				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("SensingF"));
					BlackboardComponent->SetValueAsObject(Enemy, nullptr);
				}
				 if (Actor->ActorHasTag(TeamTwoTag)) {
					// True means that they are not on the same team and we should fuck him up
					if ((PossessedTank->ActorHasTag(TeamOneTag)))
					{
						BlackboardComponent->SetValueAsObject(Enemy, Actor);
						BlackboardComponent->SetValueAsVector(LastSeenLocation, Actor->GetActorLocation());
					
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("SensingF"));
						BlackboardComponent->SetValueAsObject(Enemy, nullptr);
					}

				}	
	}

		
	

}


void ATankAIController::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}