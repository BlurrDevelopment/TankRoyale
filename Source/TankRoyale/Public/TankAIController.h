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
	// How close to the player can the AI get?
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 8000.0f;

	virtual void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnPossessedTankDeath();
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent * AIPerceptionComponent;
	class ATank * PossessedTank;
	UFUNCTION()
	void OnTargetPerceptionUpdated(class AActor * Actor,   FAIStimulus AIStimulus);
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UBlackboardData * TankBlackboardData;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UBehaviorTree * TankBehaviorTree;
	
protected:

public:
ATankAIController();
UPROPERTY(BlueprintReadWrite)
class UBlackboardComponent * BlackboardComponent;
};
