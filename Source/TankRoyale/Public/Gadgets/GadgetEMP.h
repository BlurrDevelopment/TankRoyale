// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Gadgets/TankGadget.h"
#include "GadgetEMP.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API AGadgetEMP : public ATankGadget
{
	GENERATED_BODY()
	
private:
	// Time the gadget disables the tank.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DisableTime = 2.5f;

	virtual void ActivateGadget() override;
	virtual void UseGadget() override;
	
	
};
