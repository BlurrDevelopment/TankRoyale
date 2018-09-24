// Copyright Blurr Development 2018.

#include "FindAndUsePickup.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tank.h"
EBTNodeResult::Type UFindAndUsePickup::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto AIcont = OwnerComp.GetAIOwner();
	auto BlackBComp = OwnerComp.GetBlackboardComponent();

	if (AIcont != nullptr)
	{
	auto PickUp = Cast<AActor>(BlackBComp->GetValueAsObject((KeyToGetPickupValue.SelectedKeyName)));
	AIcont->MoveToActor(PickUp, AcceptanceRadius);
	 ContTank = Cast<ATank>(AIcont->GetPawn());
	}
	if (ContTank->bOnPickup)
	{
		ContTank->UsePickup();
		BlackBComp->ClearValue(KeyToGetPickupValue.SelectedKeyName);
	}	
	return EBTNodeResult::Succeeded;
}
