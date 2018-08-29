// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Gadgets/TankGadget.h"
#include "Engine/World.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GadgetSmoke.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API AGadgetSmoke : public ATankGadget
{
	GENERATED_BODY()
	
public:
	AGadgetSmoke();

private:
	// Time the gadget is active.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float ActiveTime = 2.5f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* SmokeBlast = nullptr;

	virtual void ActivateGadget() override;
	virtual void UseGadget() override;	
};
