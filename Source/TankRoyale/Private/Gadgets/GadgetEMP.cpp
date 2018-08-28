// Copyright Blurr Development 2018.

#include "GadgetEMP.h"
#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankMovementComponent.h"

void AGadgetEMP::ActivateGadget()
{
	Super::ActivateGadget();

	UseGadget();
}

void AGadgetEMP::UseGadget()
{
	Super::UseGadget();

	// Check if the gadget hit a tank.
	auto Tank = Cast<ATank>(SurfaceStuckTo);
	if (!Tank)
	{	
		Destroy();
		return;
	}

	auto AimingComponent = Tank->FindComponentByClass<UTankAimingComponent>();
	auto MovementComponent = Tank->FindComponentByClass<UTankMovementComponent>();
	if (!AimingComponent || !MovementComponent)
	{
		Destroy();
		return;
	}

	AimingComponent->Disable(DisableTime);
	MovementComponent->Disable(DisableTime);
	Destroy();
}



