// Copyright Blurr Development 2018.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NetworkInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UNetworkInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKROYALE_API INetworkInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface. //
public:
	virtual	void Host(FString ServerName) = 0;
	virtual void Join(uint32 Index) = 0;
	virtual void GoToMainMenu() = 0;
	virtual void RequestARefresh() = 0;	
};
