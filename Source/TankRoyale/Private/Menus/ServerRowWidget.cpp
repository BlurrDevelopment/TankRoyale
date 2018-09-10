// Copyright Blurr Development 2018.

#include "ServerRowWidget.h"
#include "WidgetGameTypeManager.h"
#include "components/Button.h"
void UServerRowWidget::SetUpServerIndex(UWidgetGameTypeManager * Parent, uint32 Index)
{
	OutParent = Parent;
	OutIndex = Index;
}
void UServerRowWidget::OnServerButtonPressd()
{
	OutParent->SelectIndex(OutIndex);
}
bool UServerRowWidget::Initialize()
{
	if (Super::Initialize())
	{
		ServerButton->OnClicked.AddDynamic(this, &UServerRowWidget::OnServerButtonPressd);

		return true;
	}
	return false;
}



