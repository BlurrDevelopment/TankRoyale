// Copyright Blurr Development 2018.

#include "TankSpawnPoint.h"


// Sets default values
ATankSpawnPoint::ATankSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATankSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

