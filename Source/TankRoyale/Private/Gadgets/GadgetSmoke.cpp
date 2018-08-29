// Copyright Blurr Development 2018.

#include "GadgetSmoke.h"

AGadgetSmoke::AGadgetSmoke()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SmokeBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Smoke Blast"));
	SmokeBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SmokeBlast->bAutoActivate = false;
}


void AGadgetSmoke::ActivateGadget()
{
	Super::ActivateGadget();

	UseGadget();
}

void AGadgetSmoke::UseGadget()
{
	Super::UseGadget();
	SmokeBlast->Activate();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &Super::DestroyGadget, ActiveTime, false);
}



