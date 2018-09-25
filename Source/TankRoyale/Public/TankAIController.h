// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TankAIController.generated.h"

UCLASS()
class TANKROYALE_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
private:

	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnPossessedTankDeath();
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UAIPerceptionComponent * AIPerceptionComponent;
	class ATank * PossessedTank;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UBlackboardData * TankBlackboardData;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UBehaviorTree * TankBehaviorTree;
	
protected:

public:
ATankAIController();
UPROPERTY(BlueprintReadWrite)
class UBlackboardComponent * BlackboardComponent;
UFUNCTION()
void OnTargetPerceptionUpdated(class AActor * Actor, FAIStimulus AIStimulus);

UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
class UAISenseConfig_Sight * SightSenseConfig;
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
class UAISenseConfig_Hearing * HearingSenseConfig;
UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
class UAISenseConfig_Damage * SenseConfigDamage;
};
