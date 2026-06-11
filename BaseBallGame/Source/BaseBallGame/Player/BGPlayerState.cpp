#include "Player/BGPlayerState.h"
#include "Net/UnrealNetwork.h"
ABGPlayerState::ABGPlayerState()
{
	// allways be replicated
}

void ABGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerTryAmount);
	DOREPLIFETIME(ThisClass, MaxTryAmount);
	DOREPLIFETIME(ThisClass, IsOnGaming);
	DOREPLIFETIME(ThisClass, PlayRoomID);
}
