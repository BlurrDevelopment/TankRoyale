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

	if (!bHaveSpawnedAi)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &AGameModeDeathmatch::OnTimerEndTeam2, Timer);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGameModeDeathmatch::OnTimerEnd, Timer);
		bHaveSpawnedAi = true;
	}
}

void AGameModeDeathmatch::PostLogin(APlayerController * NewPlayer) {
	GameState = Cast<ADeathmatchGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	TeamOne = Cast<UNetworkGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetTeam1AI();
	TeamTwo = Cast<UNetworkGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetTeam2AI();
	GameState->Spawn(NewPlayer, FMath::Abs((TeamTwo - TeamOne)));
	SpawnAI();
}

void AGameModeDeathmatch::SpawnActor(AController * NewPlayer, int16 num)
{
	Controller = NewPlayer;
	Num = num;
	// TODO Multi-cast
	GetWorld()->GetTimerManager().SetTimer(TimerHandle3, this, &AGameModeDeathmatch::SpawnActorAfterDealy, Timer);
	
}
void AGameModeDeathmatch::OnTimerEnd()
{

	if (PlayerNum < TeamOne){
	ATank * Tank = Cast<ATank>(GameState->SpawnAi(1));
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
		ATank * Tank = Cast<ATank>(GameState->SpawnAi(2));
		Tank->AsAssignedToTeamSeter(true);
		PlayerNumTeam2++;
		UE_LOG(LogTemp, Warning, TEXT("team tow %d"), PlayerNumTeam2);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &AGameModeDeathmatch::OnTimerEndTeam2, Timer);
	}
}

void AGameModeDeathmatch::SpawnActorAfterDealy()
{
	if (Cast<APlayerController>(Controller))
	{
		GameState->Spawn(Controller, Num);
	}
	else
	{
		GameState->SpawnAi(Num);
		UE_LOG(LogTemp, Warning, TEXT("Ptoto"));
	}

}
