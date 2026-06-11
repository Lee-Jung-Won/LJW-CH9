#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BGGameModeBase.generated.h"

class ABGPlayerController;

USTRUCT(BlueprintType)
struct FResultData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Strike = 0;
	UPROPERTY()
	int32 Ball = 0;
	UPROPERTY()
	FString ResultString{ "" };
};

//USTRUCT(BlueprintType)
//struct FRoomData
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	int32 Playercount = 0;
//	UPROPERTY()
//	TArray<TWeakObjectPtr<ABGPlayerController>> PlayerInGame; 
//	UPROPERTY()
//	FString Answer{ "" };
//}
UCLASS()
class BASEBALLGAME_API ABGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
#pragma region CUSTOMPROPERTY
public:
	UPROPERTY()
	TMap<int, FString> RandomNumber;
#pragma endregion

#pragma region CUSTOMFUNCTION
public:
	FString MakeRandomNumber();
	bool IsStartBaseBallGame(const FString& InChatMessage);
	bool InputTextCheckerInGame(const FString& InChatMessage);
	FResultData JudgeTextStrikeOrBall(int RoomId, const FString& InChatMessage);

	void MainLogicStart(ABGPlayerController* Player, FString InChatMessage);
#pragma endregion
};
