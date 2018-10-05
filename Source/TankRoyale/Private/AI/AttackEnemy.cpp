// Copyright Blurr Development 2018.

#include "AttackEnemy.h"
#include "AIController.h"
#include "TankAimingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UAttackEnemy::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto ControlledTank = OwnerComp.GetAIOwner()->GetPawn();
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	auto Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(KeyToGetEnemyValue.SelectedKeyName));
	if (Enemy != nullptr)
	{
		if (ControlledTank->Tags[0] != Enemy->Tags[0])
		{
			if (AimingComponent != nullptr)
			{
				AimingComponent->AimAt(Enemy->GetActorLocation() + 150);
			}
			if (OwnerComp.GetAIOwner() != nullptr)
			{
				OwnerComp.GetAIOwner()->MoveToActor(Enemy, AcceptanceRadius);
			}
			if (AimingComponent != nullptr)
			{
				// FIRE!
				if (AimingComponent->GetFiringState() == EFiringState::Locked)
				{
					AimingComponent->Fire();
				}
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}
