// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindAndUsePickup.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API UFindAndUsePickup : public UBTTaskNode
{
	GENERATED_BODY()
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category = "BlackboardComponents")
		struct FBlackboardKeySelector KeyToGetPickupValue;	
private:
	class ATank * ContTank;
public:
	// How close to the player can the AI get?
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float AcceptanceRadius = 300.0f;

};
