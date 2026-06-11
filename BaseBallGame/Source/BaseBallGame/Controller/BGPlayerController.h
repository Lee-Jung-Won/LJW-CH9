#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BGPlayerController.generated.h"

class UBGInputTextWidget;
class UBGChatBoxWidget;

UCLASS()
class BASEBALLGAME_API ABGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	void SetChatMessagefromTextBox(const FString& InChatMessage);
	void PrintChatMessage(const FString& InChatMessage);
	void PrintBroadCastMessage(const FString& InChatMessage);
	void WinnerBroadcast(const FString& InChatMessage);

	UFUNCTION(Server, Reliable)
	void ServerRPCSendChatMessage(const FString& InChatMessage);
	UFUNCTION(Client, Reliable)
	void ClientRPCTakeChatMessage(const FString& InChatMessage);
	UFUNCTION(Client, Reliable)
	void ClientRPCTakeBlockMessage();
	UFUNCTION(Client, Reliable)
	void ClientRPCTakeReleaseMessage();


public:

	UPROPERTY(EditDefaultsOnly, Category = "BaseBallGame|UI")
	TSubclassOf<UBGInputTextWidget> InputWidgetClass;
	UPROPERTY()
	TObjectPtr<UBGInputTextWidget> InputWidgetInstance;
	UPROPERTY(EditDefaultsOnly, Category = "BaseBallGame|UI")
	TSubclassOf<UBGChatBoxWidget> ChatBoxWidgetClass;
	UPROPERTY()
	TObjectPtr<UBGChatBoxWidget> ChatBoxWidgetInstance;
private:
	FString ChatMessage;

};
