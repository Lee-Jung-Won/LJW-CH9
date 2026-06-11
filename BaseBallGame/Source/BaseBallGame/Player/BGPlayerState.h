#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BGPlayerState.generated.h"


UCLASS()
class BASEBALLGAME_API ABGPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABGPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	// In PlayerState PROPERTY
	// UPROPERTY(ReplicatedUsing = OnRep_PlayerName)
	// FString PlayerNamePrivate;

	UPROPERTY(Replicated)
	int32 PlayerTryAmount{ 0 };
	UPROPERTY(Replicated)
	int32 MaxTryAmount{ 3 };
	UPROPERTY(Replicated)
	uint8 IsOnGaming = false;
	UPROPERTY(Replicated)
	int32 PlayRoomID{ 0 };


};
