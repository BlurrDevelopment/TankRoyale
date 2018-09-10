// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetGameTypeManager.generated.h"

/**
 * 
 */
USTRUCT()
struct FServerData {
	GENERATED_BODY();
	FString  ServerName;
	uint16	CurrentPlayers;
	uint16 MaxPlayers;
	FString HostName;
};
// TODO rename class to host or join menu
UCLASS()
class TANKROYALE_API UWidgetGameTypeManager : public UUserWidget
{
GENERATED_BODY()
public:	
void TearDown();
void SetNetworkInterface(class INetworkInterface * INetworkInterface);
//TODO make it take an enum
UPROPERTY(EditDefaultsOnly)
FString GameMode;
UFUNCTION()
void JoinGamePressd();
UFUNCTION()
void HostPressd();
void SelectIndex(uint32 Index);
void UpdateChildern();
void SetServerList(TArray<FServerData> ServersData);
protected:
class INetworkInterface * NetworkInterface;
virtual bool Initialize();
private:
UPROPERTY(meta = (BindWidget))
class UButton *  Host;
UPROPERTY(meta = (BindWidget))
class UButton *  Join;
TOptional<uint32> ServerIndex;
UPROPERTY(meta = (BindWidget))
class UScrollBox * ScrollServer;
TSubclassOf<class UServerRowWidget> ServerWidgetSub;
class UServerRowWidget * SelectedWidget;
};
