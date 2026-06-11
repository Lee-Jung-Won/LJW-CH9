#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGInputTextWidget.generated.h"

class UEditableTextBox;
class UScrollBox;
class UTextBlock;
class UWidgetAnimation;

UCLASS()
class BASEBALLGAME_API UBGInputTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetChildWidgetToScrollBox(UUserWidget* SetChildeWidget);
	void SetScrollWidgetToEnd();
	UFUNCTION()
	void TextBoxOnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void SetBroadCastTextBlock(const FString& InChatMessage);
	void SetBroadCastWinnerTextBlock(const FString& InChatMessage);

	void SetInputBoxBeBlock();
	void SetInputBoxBeRelease();
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_InputBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BroadcastTextBlock;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BroadcastWinner;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Disappear;
};
