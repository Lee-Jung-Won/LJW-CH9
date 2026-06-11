#include "UI/BGChatBoxWidget.h"
#include "Components/TextBlock.h"

void UBGChatBoxWidget::SetChatText(const FString& InChatMessage) const
{
	if (!IsValid(ChatTextBlock)) return;
	ChatTextBlock->SetText(FText::FromString(InChatMessage));
}
