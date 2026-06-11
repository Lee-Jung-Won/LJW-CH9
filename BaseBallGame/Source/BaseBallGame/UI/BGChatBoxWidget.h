#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGChatBoxWidget.generated.h"

UCLASS()
class BASEBALLGAME_API UBGChatBoxWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetChatText(const FString& InChatMessage) const;
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatTextBlock;
};
