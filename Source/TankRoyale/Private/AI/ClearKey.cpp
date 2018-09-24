// Copyright Blurr Development 2018.

#include "ClearKey.h"
#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UClearKey::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->ClearValue(KeyToClear.SelectedKeyName);
	return EBTNodeResult::Succeeded;
}
