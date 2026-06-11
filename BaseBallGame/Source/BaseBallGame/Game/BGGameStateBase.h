#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BGGameStateBase.generated.h"

class ABGPlayerController;

UCLASS()
class BASEBALLGAME_API ABGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:

	UFUNCTION(NetMulticast, Reliable)
	void BroadCastPlayerToInPlayer(const FString& InJoinMessage);
	UFUNCTION(NetMulticast, Reliable)
	void BroadCastGameResultToInPlayer(const FString& InJoinMessage);
};
