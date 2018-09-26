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
int16 GetTeamOneAi();
int16 GetTeamTwoAi();
void TearDown();
void SetNetworkInterface(class INetworkInterface * INetworkInterface);
UFUNCTION()
void OnHostServerButtonPressd();
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
UFUNCTION()
void RefreshServerList();
UFUNCTION()
void GoToJoinMenu();
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
UPROPERTY(EditAnywhere)
TSubclassOf<class UServerRowWidget> ServerWidgetSub;
class UServerRowWidget * SelectedWidget;
UPROPERTY(meta = (BindWidget))
class UButton *  HostServer;
UPROPERTY(meta = (BindWidget))
class UEditableTextBox * ServerNameBox;
UPROPERTY(meta = (BindWidget))
class UWidgetSwitcher * MenuSwitcher;
UPROPERTY(meta = (BindWidget))
UWidget * HostMenu;
UPROPERTY(meta = (BindWidget))
class UButton * Refresh;
UPROPERTY(meta = (BindWidget))
class UButton * BackToJoinMenu;
UPROPERTY(meta = (BindWidget))
class UComboBoxString * ComboBoxAiTeamOne;
UPROPERTY(meta = (BindWidget))
class UComboBoxString * ComboBoxAITeamTwo;
private:
int16 TeamOneAi;
int16 TeamTwoAi;
};
