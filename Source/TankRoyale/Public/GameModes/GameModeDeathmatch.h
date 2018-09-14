// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeDeathmatch.generated.h"



/**
 * 
 */
UCLASS()
class TANKROYALE_API AGameModeDeathmatch : public AGameModeBase
{
	GENERATED_BODY()
protected:
	void PostLogin(APlayerController * NewPlayer) override;
private:
	int16 PlayerNumber = 0;
	TArray<AActor *> SpawnPoints;
	AActor * SpawnPointAtIndex;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATank> TankSubClass;
};


