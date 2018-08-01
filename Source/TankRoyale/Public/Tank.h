// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class TANKROYALE_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Called by the engine when damage is dealt
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Returns current health as a percentage of starting health, between 0 and 100
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UPROPERTY(BlueprintAssignable, Category = "Death")
	FTankDelegate OnDeath;

private:
	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth;
	
public:
	// Team stuff
	UPROPERTY(BlueprintReadOnly, Category = "Team")
	int32 Team = 0; // 0 = No team

	// Get the tanks team. 0 is No Team
	UFUNCTION(BlueprintPure, Category = "Team")
	int32 GetTankTeam() const { return Team; }

	// Set the tanks team
	void SetTankTeam(int32 NewTeam);
};
