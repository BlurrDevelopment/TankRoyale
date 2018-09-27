// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "NetworkInterface.h"
#include "NetworkGameInstance.generated.h"





UCLASS()
class TANKROYALE_API UNetworkGameInstance : public UGameInstance , public INetworkInterface
{
GENERATED_BODY()

virtual void Init();
UFUNCTION(Exec)
void Host(FString ServerName)  override;
void CreateAndHost(FName SessionName, bool bIsSessionReady);
UFUNCTION(BlueprintCallable)
void Setwidget(class UWidgetGameTypeManager * WidgetToSet);
UFUNCTION(Exec)
void Join(uint32 Index) override;
void GoToMainMenu() override;
void RequestARefresh() override;
class UWidgetGameTypeManager * Menu;
int16 TeamOneAi = 0;
int16 TeamTwoAi = 0;
public:
int16 GetTeam1AI();
int16 GetTeam2AI();
private:
	bool bIsSessionCrated;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr < class FOnlineSessionSearch > SearchSettings;
	void OnDestroySession(FName SessionName, bool bIsSessionDestroy);
	void OnFindSessionsComplete(bool Success);
	void CreateSession(FString ServerName);
	TArray<FString> SearchResultArray;
	FString CurnantServer;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	

};
