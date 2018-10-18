// Copyright Blurr Development 2018.

#include "GameModeDeathmatch.h"
#include "DeathmatchGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Menus/WidgetGameTypeManager.h"
#include "GameInstances/NetworkGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Public/TimerManager.h"
#include "Tank.h"
void AGameModeDeathmatch::SpawnAI()
{
	 TeamOne = Cast<UNetworkGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetTeam1AI();
	 TeamTwo = Cast<UNetworkGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetTeam2AI();
	if (!bHaveSpawnedAi)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGameModeDeathmatch::OnTimerEndTeam2, Timer);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGameModeDeathmatch::OnTimerEnd, Timer);
		bHaveSpawnedAi = true;
	}
}

void AGameModeDeathmatch::PostLogin(APlayerController * NewPlayer) {
	GameState = Cast<ADeathmatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->Spawn(NewPlayer, GameState->PlayerNumber);
	SpawnAI();
}

AActor * AGameModeDeathmatch::SpawnActor(TSubclassOf<AActor> ActorToSpawn, FVector SpawnLocation, FRotator SpawnRotation)
{
	UWorld * World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null"));
	}
	return World->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation);
}
void AGameModeDeathmatch::OnTimerEnd()
{

	if (PlayerNum < TeamOne){
	ATank * Tank = Cast<ATank>(GameState->SpawnAi(PlayerNum));
	Tank->Tags.Empty();
	GameState->AssignTankToTeamByN(1, Tank);
	Tank->AsAssignedToTeamSeter(true);
	PlayerNum++;
	UE_LOG(LogTemp, Warning, TEXT("2 + 2 %d"), PlayerNum);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGameModeDeathmatch::OnTimerEnd, Timer);
	}
}

void AGameModeDeathmatch::OnTimerEndTeam2()
{
	
	if (PlayerNumTeam2 < TeamTwo)
	{
		ATank * Tank = Cast<ATank>(GameState->SpawnAi(PlayerNumTeam2 + 5));
		Tank->Tags.Empty();
		GameState->AssignTankToTeamByN(2, Tank);
		Tank->AsAssignedToTeamSeter(true);
		PlayerNumTeam2++;
		UE_LOG(LogTemp, Warning, TEXT("team tow %d"), PlayerNumTeam2);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGameModeDeathmatch::OnTimerEndTeam2, Timer);
	}
}
