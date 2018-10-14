// Copyright Blurr Development 2018.

#include "ServerRowWidget.h"
#include "WidgetGameTypeManager.h"
#include "components/Button.h"
void UServerRowWidget::SetUpServerIndex(UWidgetGameTypeManager * Parent, uint32 Index)
{
	if (!IsGuide)
	{
		OutParent = Parent;
		OutIndex = Index;
	}
}
void UServerRowWidget::OnServerButtonPressd()
{
	if (!IsGuide)
	{

		OutParent->SelectIndex(OutIndex);
	}
}
bool UServerRowWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (!IsGuide)
		{


			ServerButton->OnClicked.AddDynamic(this, &UServerRowWidget::OnServerButtonPressd);
		}
		return true;
	}
	return false;
}



