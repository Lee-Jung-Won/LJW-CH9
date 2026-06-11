#include "Controller/BGPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BGInputTextWidget.h"
#include "UI/BGChatBoxWidget.h"
#include "Components/ScrollBox.h"
#include "Game/BGGameModeBase.h"

void ABGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController()) return;
	// this line is in client

	FInputModeUIOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	if (!IsValid(InputWidgetClass)) return;
	InputWidgetInstance = CreateWidget<UBGInputTextWidget>(this, InputWidgetClass);

	if (!IsValid(InputWidgetInstance)) return;
	InputWidgetInstance->AddToViewport();

}
void ABGPlayerController::SetChatMessagefromTextBox(const FString& InChatMessage)
{
	ChatMessage = InChatMessage;
	//PrintChatMessage(InChatMessage);
	ServerRPCSendChatMessage(InChatMessage);
}

void ABGPlayerController::PrintChatMessage(const FString& InChatMessage)
{
	if (IsLocalController())
	{
		if (!IsValid(InputWidgetClass)) return;
		ChatBoxWidgetInstance = CreateWidget<UBGChatBoxWidget>(this, ChatBoxWidgetClass);

		if (!IsValid(InputWidgetInstance)) return;

		ChatBoxWidgetInstance->SetChatText(InChatMessage);
		InputWidgetInstance->SetChildWidgetToScrollBox(ChatBoxWidgetInstance);
		InputWidgetInstance->SetScrollWidgetToEnd();
	}
	//UKismetSystemLibrary::PrintString(this, InChatMessage);
}

void ABGPlayerController::PrintBroadCastMessage(const FString& InChatMessage)
{
	// broadcast widget
	InputWidgetInstance->SetBroadCastTextBlock(InChatMessage);
}

void ABGPlayerController::WinnerBroadcast(const FString& InChatMessage)
{
	InputWidgetInstance->SetBroadCastWinnerTextBlock(InChatMessage);
}

void ABGPlayerController::ClientRPCTakeReleaseMessage_Implementation()
{
	InputWidgetInstance->SetInputBoxBeRelease();
}

void ABGPlayerController::ClientRPCTakeBlockMessage_Implementation()
{
	InputWidgetInstance->SetInputBoxBeBlock();
}

void ABGPlayerController::ServerRPCSendChatMessage_Implementation(const FString& InChatMessage)
{
	// is in server
	ABGGameModeBase* GM = Cast<ABGGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(GM)) return;
	GM->MainLogicStart(this, InChatMessage);
}

void ABGPlayerController::ClientRPCTakeChatMessage_Implementation(const FString& InChatMessage)
{
	// is in client
	PrintChatMessage(InChatMessage);
}
