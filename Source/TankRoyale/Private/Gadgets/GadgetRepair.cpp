// Copyright Blurr Development 2018.

#include "GadgetRepair.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"


void AGadgetRepair::ActivateGadget()
{
	Super::ActivateGadget();

	UseGadget();
}

void AGadgetRepair::UseGadget()
{
	Super::UseGadget();
	if (Cast<ATank>(SurfaceStuckTo)) Cast<ATank>(SurfaceStuckTo)->Repair(RepairAmount);
	Destroy();
}
