// Copyright Blurr Development 2018.

#include "RunEqsTask.h"
#include "Engine/World.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
EBTNodeResult::Type URunEqsTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	MyQueryRequest = FEnvQueryRequest(EqsToRun, this);
	if (EqsToRun != nullptr)
	{
		
			MyQueryRequest.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &URunEqsTask::MyQueryFinished);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(KeyToGetEqsValue.SelectedKeyName, ResultVector);
			return EBTNodeResult::Succeeded;
		
	}
	return EBTNodeResult::Failed;
}

void URunEqsTask::MyQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	ResultVector = Result->GetItemAsLocation(0);
}
