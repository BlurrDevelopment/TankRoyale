// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Gadgets/TankGadget.h"
#include "GadgetRepair.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API AGadgetRepair : public ATankGadget
{
	GENERATED_BODY()
	
private:
	float RepairAmount = 40.0f;

	virtual void ActivateGadget() override;
	virtual void UseGadget() override;
};
