// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API UAttackEnemy : public UBTTaskNode
{
	GENERATED_BODY()
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category = "BlackboardComponents")
		struct FBlackboardKeySelector KeyToGetEnemyValue;
public:
	// How close to the player can the AI get?
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float AcceptanceRadius = 3000.0f;
	
};
