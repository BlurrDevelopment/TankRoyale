// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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
protected:
};
