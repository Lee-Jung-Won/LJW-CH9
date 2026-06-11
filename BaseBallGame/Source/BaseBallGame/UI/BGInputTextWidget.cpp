#include "BGInputTextWidget.h"
#include "Components/EditableTextBox.h"
#include "Controller/BGPlayerController.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Animation/WidgetAnimation.h"

void UBGInputTextWidget::NativeConstruct()
{
	if (!(EditableTextBox_InputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::TextBoxOnTextCommitted)))
	{
		EditableTextBox_InputBox->OnTextCommitted.AddDynamic(this, &ThisClass::TextBoxOnTextCommitted);
	}
}

void UBGInputTextWidget::NativeDestruct()
{
	if (EditableTextBox_InputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::TextBoxOnTextCommitted))
	{
		EditableTextBox_InputBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::TextBoxOnTextCommitted);
	}
}

void UBGInputTextWidget::SetChildWidgetToScrollBox(UUserWidget* SetChildeWidget)
{
	ScrollBox->AddChild(SetChildeWidget);
}

void UBGInputTextWidget::SetScrollWidgetToEnd()
{
	ScrollBox->ScrollToEnd();
}

void UBGInputTextWidget::TextBoxOnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		ABGPlayerController* Player = Cast<ABGPlayerController>(GetOwningPlayer());
		if (!IsValid(Player)) return;
		Player->SetChatMessagefromTextBox(Text.ToString());

		EditableTextBox_InputBox->SetText(FText::GetEmpty());
	}
}

void UBGInputTextWidget::SetBroadCastTextBlock(const FString& InChatMessage)
{
	BroadcastTextBlock->SetText(FText::FromString(InChatMessage));
	PlayAnimation(Disappear);
}

void UBGInputTextWidget::SetBroadCastWinnerTextBlock(const FString& InChatMessage)
{
	BroadcastWinner->SetText(FText::FromString(InChatMessage));
}

void UBGInputTextWidget::SetInputBoxBeBlock()
{
	EditableTextBox_InputBox->SetIsEnabled(false);

	if (!IsValid(GetOwningPlayer())) return;

	FInputModeGameOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
}

void UBGInputTextWidget::SetInputBoxBeRelease()
{
	EditableTextBox_InputBox->SetIsEnabled(true);

	if (!IsValid(GetOwningPlayer())) return;

	FInputModeUIOnly InputMode;
	GetOwningPlayer()->SetInputMode(InputMode);
}
