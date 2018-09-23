// Copyright Blurr Development 2018.

#include "AttackEnemy.h"
#include "AIController.h"
#include "TankAimingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UAttackEnemy::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto ControlledTank = OwnerComp.GetAIOwner()->GetPawn();
	// Aim towards the player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	auto Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KeyToGetEnemyValue.SelectedKeyName));
	AimingComponent->AimAt(Enemy->GetActorLocation() + 150);
	OwnerComp.GetAIOwner()->MoveToActor(Enemy, AcceptanceRadius); 
	// FIRE!
	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire();
	}
	return EBTNodeResult::Succeeded;
}
