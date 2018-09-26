// Copyright Blurr Development 2018.

#include "WidgetGameTypeManager.h"
#include "NetworkInterface.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "ServerRowWidget.h"
#include "Components/ComboBoxString.h"
void UWidgetGameTypeManager::SetNetworkInterface(INetworkInterface * INetworkInterface)
{
	NetworkInterface = INetworkInterface;
}
bool UWidgetGameTypeManager::Initialize() {
	bool bSuccess = Super::Initialize();
	if (bSuccess)
	{

		if (Host != nullptr)
		{
			Host->OnClicked.AddDynamic(this, &UWidgetGameTypeManager::HostPressd);
		}
		if (Join != nullptr)
		{
			Join->OnClicked.AddDynamic(this, &UWidgetGameTypeManager::JoinGamePressd);
		}
		if (HostServer != nullptr)
		{
			HostServer->OnClicked.AddDynamic(this, &UWidgetGameTypeManager::OnHostServerButtonPressd);
		}
		if (Refresh != nullptr)
		{
			Refresh->OnClicked.AddDynamic(this, &UWidgetGameTypeManager::RefreshServerList);
		}
		if (BackToJoinMenu != nullptr)
		{
			BackToJoinMenu->OnClicked.AddDynamic(this, &UWidgetGameTypeManager::GoToJoinMenu);
		}
		
		
		return true;
	}
	return false;

}
int16 UWidgetGameTypeManager::GetTeamOneAi()
{
	return TeamOneAi;
}
int16 UWidgetGameTypeManager::GetTeamTwoAi()
{
	return TeamTwoAi;
}
void UWidgetGameTypeManager::OnHostServerButtonPressd() {
	if (NetworkInterface != nullptr) {
		if (ComboBoxAiTeamOne->GetSelectedOption() == "0")
		{
			TeamOneAi = 0;
		}
		if (ComboBoxAiTeamOne->GetSelectedOption() == "1")
		{
			TeamOneAi = 1;
		}

		if (ComboBoxAiTeamOne->GetSelectedOption() == "2")
		{
			TeamOneAi = 2;
		}

		if (ComboBoxAiTeamOne->GetSelectedOption() == "3")
		{
			TeamOneAi = 3;
		}

		if (ComboBoxAiTeamOne->GetSelectedOption() == "4")
		{
			TeamOneAi = 4;
		}
		if (ComboBoxAiTeamOne->GetSelectedOption() == "5")
		{
			TeamOneAi = 5;
		}
		if (ComboBoxAITeamTwo->GetSelectedOption() == "0")
		{
			TeamTwoAi = 0;
		}
		if (ComboBoxAITeamTwo->GetSelectedOption() == "1")
		{
			TeamTwoAi = 1;
		}

		if (ComboBoxAITeamTwo->GetSelectedOption() == "2")
		{
			TeamTwoAi = 2;
		}

		if (ComboBoxAITeamTwo->GetSelectedOption() == "3")
		{
			TeamTwoAi = 3;
		}

		if (ComboBoxAITeamTwo->GetSelectedOption() == "4")
		{
			TeamTwoAi = 4;
		}
		if (ComboBoxAITeamTwo->GetSelectedOption() == "5")
		{
			TeamTwoAi = 5;
		}

		NetworkInterface->Host(ServerNameBox->GetText().ToString());
	}
}

void UWidgetGameTypeManager::TearDown()
{
	this->RemoveFromViewport();
	APlayerController * Player = GetWorld()->GetFirstPlayerController();
	FInputModeGameOnly  InputModeData;
	Player->bShowMouseCursor = false;
	Player->SetInputMode(InputModeData);
}

void UWidgetGameTypeManager::JoinGamePressd()
{
	if (NetworkInterface != nullptr) {

		if (ServerIndex.IsSet())
		{
			UE_LOG(LogTemp, Warning, TEXT("ServerIndex %i "), ServerIndex.GetValue());
			NetworkInterface->Join(ServerIndex.GetValue());
		}
	}
}



void UWidgetGameTypeManager::HostPressd()
{
		MenuSwitcher->SetActiveWidget(HostMenu);
}

void UWidgetGameTypeManager::SelectIndex(uint32 Index)
{
	ServerIndex = Index;
	UpdateChildern();
}

void UWidgetGameTypeManager::UpdateChildern()
{
	if (SelectedWidget != nullptr) {
		SelectedWidget->Selected = false;
	}
	SelectedWidget = Cast<UServerRowWidget>(ScrollServer->GetChildAt(ServerIndex.GetValue()));
	SelectedWidget->Selected = true;
}

void UWidgetGameTypeManager::SetServerList(TArray<FServerData> ServersData)
{
	UWorld * World = GetWorld();
	if (World != nullptr)
	{
		if (ScrollServer != nullptr)
		{
			ScrollServer->ClearChildren();
			int32	i = 0;
			for (FServerData &ServerData : ServersData)
			{
				UServerRowWidget *ServerWidget = CreateWidget<UServerRowWidget>(World, ServerWidgetSub);
				if (ServerWidget != nullptr) {

					ServerWidget->ServerName->SetText(FText::FromString(ServerData.ServerName));
					ServerWidget->Host->SetText(FText::FromString(ServerData.HostName));
					FString CurrentToMaxPlayres = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
					ServerWidget->PlayersNumber->SetText(FText::FromString(CurrentToMaxPlayres));
					ScrollServer->AddChild(ServerWidget);
					ServerWidget->SetUpServerIndex(this, i);
					++i;

				}
			}
		}
	}

}

void UWidgetGameTypeManager::RefreshServerList()
{
	if (NetworkInterface != nullptr) {
		NetworkInterface->RequestARefresh();
	}
}

void UWidgetGameTypeManager::GoToJoinMenu()
{
	MenuSwitcher->SetActiveWidget(Refresh);
}
