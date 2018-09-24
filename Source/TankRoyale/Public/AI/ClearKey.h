// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ClearKey.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API UClearKey : public UBTTaskNode
{
	GENERATED_BODY()
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category = "BlackboardComponents")
		struct FBlackboardKeySelector KeyToClear;
	
	
};
