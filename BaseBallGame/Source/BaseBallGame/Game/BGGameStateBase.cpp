#include "Game/BGGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/BGPlayerController.h"
void ABGGameStateBase::BroadCastPlayerToInPlayer_Implementation(const FString& InJoinMessage)
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!IsValid(PC)) return;

		if (ABGPlayerController* BGPC = Cast<ABGPlayerController>(PC))
		{
			BGPC->PrintBroadCastMessage(InJoinMessage);
		}
	}
}

void ABGGameStateBase::BroadCastGameResultToInPlayer_Implementation(const FString& InJoinMessage)
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!IsValid(PC)) return;

		if (ABGPlayerController* BGPC = Cast<ABGPlayerController>(PC))
		{
			BGPC->WinnerBroadcast(InJoinMessage);
		}
	}
}
