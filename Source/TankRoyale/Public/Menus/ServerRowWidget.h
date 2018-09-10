// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKROYALE_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock * ServerName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayersNumber;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Host;
	UPROPERTY(meta = (BindWidget))
		class UButton * ServerButton;
	UFUNCTION()
		void OnServerButtonPressd();
	void SetUpServerIndex(class UWidgetGameTypeManager * Parent, uint32 Index);
protected:
	virtual bool Initialize();
private:
	class UWidgetGameTypeManager * OutParent;
	uint32 OutIndex;

};
