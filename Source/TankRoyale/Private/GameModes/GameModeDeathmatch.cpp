// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TankSpawnPoint.h"
#include "Tank.h"
void AGameModeDeathmatch::PostLogin(APlayerController * NewPlayer) {
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATankSpawnPoint::StaticClass(), SpawnPoints);
	if (SpawnPoints.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("nope"));
		return;
	}
	AActor * PointToSpawn = SpawnPoints[PlayerNumber];
	ATank * Tank = GetWorld()->SpawnActor<ATank>(TankSubClass, PointToSpawn->GetActorLocation(), FRotator(0, 0, 0));
	//Tank->PossessedBy(NewPlayer);
	NewPlayer->Possess(Tank);
	PlayerNumber++;
}