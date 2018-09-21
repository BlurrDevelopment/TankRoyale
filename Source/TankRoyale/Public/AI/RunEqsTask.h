// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "RunEqsTask.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API URunEqsTask : public UBTTaskNode
{
	GENERATED_BODY()
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category = "BlackboardComponents")
		struct FBlackboardKeySelector KeyToGetEqsValue;
	UPROPERTY(EditAnywhere, Category = "Eqs")
		class UEnvQuery * EqsToRun;
	 FEnvQueryRequest MyQueryRequest;
	void MyQueryFinished(TSharedPtr< FEnvQueryResult> Result);
private:
	FVector ResultVector;
};
