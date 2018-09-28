// Copyright Blurr Development 2018.
#include "NetworkGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Containers/Array.h"
#include "Engine/Engine.h"
#include "Menus/WidgetGameTypeManager.h"
const static FName SessionName = NAME_GameSession;

void UNetworkGameInstance::Init() {
	Super::Init();
	IOnlineSubsystem *OnlineSubsystem;
	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem != nullptr)
	{

		UE_LOG(LogTemp, Warning, TEXT("%s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();

		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnDestroySession);
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::CreateAndHost);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetworkGameInstance::OnJoinSessionComplete);
			UE_LOG(LogTemp, Warning, TEXT("start searching Sessions"));

		}


	}
}
void UNetworkGameInstance::RequestARefresh()
{
	SearchSettings = MakeShareable(new FOnlineSessionSearch());
	if (SearchSettings.IsValid()) {
		SearchSettings->MaxSearchResults = 100;
		SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SearchSettings.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT(" RequestARefresh"));
	}

}

int16 UNetworkGameInstance::GetTeam1AI()
{
	return TeamOneAi;
}

int16 UNetworkGameInstance::GetTeam2AI()
{
	return 	TeamTwoAi; 
}



void UNetworkGameInstance::OnDestroySession(FName SessionName, bool bIsSessionDestroy)
{
	if (bIsSessionDestroy) {

		CreateSession(SessionName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Destroy SessionInterface"));

	}
}
void UNetworkGameInstance::OnFindSessionsComplete(bool Success)
{

	if (Success && SearchSettings.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> ServersData;
		for (const FOnlineSessionSearchResult &SearchResult : SearchSettings->SearchResults)
		{

			FString tag;
			if (SearchResult.Session.SessionSettings.Get(TEXT("Test"), tag)) {
				UE_LOG(LogTemp, Warning, TEXT(" tag %s "), *tag);
			}
			FServerData ServerData;
			ServerData.ServerName = tag;
			ServerData.HostName = SearchResult.Session.OwningUserName;
			ServerData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = SearchResult.Session.SessionSettings.NumPublicConnections - SearchResult.Session.NumOpenPublicConnections;
			ServersData.Add(ServerData);



		}

		Menu->SetServerList(ServersData);


	}
	else{ UE_LOG(LogTemp, Warning, TEXT(" no found")); }
	
}
void UNetworkGameInstance::Join(uint32 Index)
{

	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	if (SessionInterface.IsValid())
	{

		if (SearchSettings.IsValid())
		{
			SessionInterface->JoinSession(0, SessionName, SearchSettings->SearchResults[Index]);
			UE_LOG(LogTemp, Warning, TEXT("OnJoinSession calld"));
		}
	}
}

void UNetworkGameInstance::GoToMainMenu()
{
}



void UNetworkGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));
	FString Address;
	if (!SessionInterface.IsValid())
	{
		return;
	}
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		return;

	}
	APlayerController * Player = GetFirstLocalPlayerController();
	if (Player != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete %s"), *Address);
		Player->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}

	UEngine * Engine = GetEngine();
	if (Engine != nullptr)
	{

		Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("Joined %s"), *Address));
	}
}

void UNetworkGameInstance::CreateSession(FString ServerName)
{
	FOnlineSessionSettings OnlineSessionSettings;
	if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL") {
		OnlineSessionSettings.bIsLANMatch = true;
	}
	else
	{
		OnlineSessionSettings.bIsLANMatch = false;
	}
	OnlineSessionSettings.NumPublicConnections = 5;
	OnlineSessionSettings.bShouldAdvertise = true;
	OnlineSessionSettings.bUsesPresence = true;
	OnlineSessionSettings.Set(TEXT("Test"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SessionName, OnlineSessionSettings);
}



void UNetworkGameInstance::Host(FString ServerName)
{
	if (SessionInterface.IsValid())
	{
		auto CurrentSession = SessionInterface->GetNamedSession(SessionName);
		if (CurrentSession != nullptr) {
			SessionInterface->DestroySession(SessionName);

		}
		else {
			CreateSession(ServerName);
		}

	}



}

void UNetworkGameInstance::CreateAndHost(FName SessionName, bool bIsSessionReady)
{
	if (!bIsSessionReady) {
		return;
	}
	TeamOneAi = Menu->GetTeamOneAi();
	TeamTwoAi = Menu->GetTeamTwoAi();
	if (Menu != nullptr) {

		Menu->TearDown();
	}

	UEngine * Engine = GetEngine();
	if (Engine != nullptr)
	{

		Engine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Hosting"));
		GetWorld()->ServerTravel("/Game/_Levels/Maps/Map2?listen");
	}
	

}

void UNetworkGameInstance::Setwidget(UWidgetGameTypeManager * WidgetToSet)
{
	Menu = WidgetToSet;
	Menu->SetNetworkInterface(this);
}



