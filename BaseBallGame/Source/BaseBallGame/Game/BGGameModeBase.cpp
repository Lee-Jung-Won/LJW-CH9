#include "Game/BGGameModeBase.h"
#include "Game/BGGameStateBase.h"
#include "Player/BGPlayerState.h"
#include "Controller/BGPlayerController.h"
#include "EngineUtils.h"
void ABGGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    //RandomNumber = MakeRandomNumber();
    //UE_LOG(LogTemp, Error, TEXT("%s"), *RandomNumber);
}

void ABGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    // GameMode is in Server.
    // TODO:: MakeID in PC->PS
    static int CountPlayer = 0; // static is kick

    ABGPlayerState* CustomPlayerState = NewPlayer->GetPlayerState<ABGPlayerState>();
    if (!IsValid(CustomPlayerState)) return;

    CustomPlayerState->SetPlayerName(FString::Printf(TEXT("Player %d"), ++CountPlayer));

    ABGGameStateBase* CustomGameState = GetGameState<ABGGameStateBase>();
    if (!IsValid(CustomGameState)) return;

    FString WelcomeToJoin = CustomPlayerState->GetPlayerName() + TEXT(" has joined the chatting.");
    CustomGameState->BroadCastPlayerToInPlayer(WelcomeToJoin);

}

FString ABGGameModeBase::MakeRandomNumber()
{
    FString CustomRandomNumber{""};
    TArray<int> CollectNumberByOne;
    for (int i = 1; i <= 9; ++i)
    {
        CollectNumberByOne.Add(i);
    }

    for (int j = 0; j < 3; ++j) // Make 3 Num
    {
        int RandomIndex = FMath::RandRange(0, CollectNumberByOne.Num() - 1);
        CustomRandomNumber += FString::FromInt(CollectNumberByOne[RandomIndex]);
        CollectNumberByOne.RemoveAt(RandomIndex);
    }

    return CustomRandomNumber;
}

bool ABGGameModeBase::IsStartBaseBallGame(const FString& InChatMessage)
{
    return (InChatMessage.Contains(TEXT("BaseBall"), ESearchCase::IgnoreCase)) ? 1 : 0;
}

bool ABGGameModeBase::InputTextCheckerInGame(const FString& InChatMessage)
{
    // is game start? == trigger
    if (!(InChatMessage.Len() == 3)) return false;

    for (TCHAR Ch : InChatMessage)
    {
        if (!FChar::IsDigit(Ch))
        {
            return false;
        }
    }

    TSet<TCHAR> CharSet;
    for (TCHAR Ch : InChatMessage)
    {
        if (CharSet.Contains(Ch))
        {
            return false;
        }
        CharSet.Add(Ch);
    }

    // pass conditions
    return true;
}

FResultData ABGGameModeBase::JudgeTextStrikeOrBall(int RoomId, const FString& InChatMessage)
{
    FString RoomRandomNumber = RandomNumber[RoomId];
    FResultData ResultData;
    int32 StrikeCount = 0;
    int32 BallCount = 0;

    for (int i = 0; i < InChatMessage.Len(); ++i)
    {
        

        if (RoomRandomNumber[i] == InChatMessage[i])
        {
            StrikeCount++;
            continue;
        }

        if (RoomRandomNumber.Contains(FString::Chr(InChatMessage[i])))
        {
            BallCount++;
            continue;
        }
    }

    FString FinalResultString{""};
    if (BallCount || StrikeCount)
    {
        FinalResultString = FString::Printf(TEXT("S: %d B: %d"), StrikeCount, BallCount);
    }
    else
    {
        FinalResultString = FString::Printf(TEXT("OUT"));
    }

    ResultData.ResultString = FinalResultString;
    ResultData.Strike = StrikeCount;
    ResultData.Ball = BallCount;

    return ResultData;
}

void ABGGameModeBase::MainLogicStart(ABGPlayerController* Player, FString InChatMessage)
{
    ABGPlayerState* CustomPlayerState = Player->GetPlayerState<ABGPlayerState>();
    FString JudgeResult{ "" };
    FResultData GetResultData;

    if (CustomPlayerState->IsOnGaming)
    {
        if (InputTextCheckerInGame(InChatMessage))
        {
            GetResultData = JudgeTextStrikeOrBall(CustomPlayerState->PlayRoomID, InChatMessage);

            JudgeResult = GetResultData.ResultString; // S / B
            CustomPlayerState->PlayerTryAmount++;

            // try count add
            JudgeResult += FString::Printf(TEXT("  [ %d / %d ]"), CustomPlayerState->PlayerTryAmount, CustomPlayerState->MaxTryAmount);
        }
        else
        {
            InChatMessage = TEXT("INPUT TEXT IS INVALID.");
        }
    }
    else if (IsStartBaseBallGame(InChatMessage))
    {
        static int GameRoomID = 0;
        ++GameRoomID;
        RandomNumber.Add(GameRoomID, MakeRandomNumber());

        UE_LOG(LogTemp, Error, TEXT("Room%d: %s"), GameRoomID, *RandomNumber[GameRoomID]);

        for (TActorIterator<ABGPlayerController> It(GetWorld()); It; ++It)
        {
            ABGPlayerState* InGamePlayerState = It->GetPlayerState<ABGPlayerState>();
            if (!InGamePlayerState->IsOnGaming)
            {
                InGamePlayerState->IsOnGaming = true;
                InGamePlayerState->PlayRoomID = GameRoomID;
            }
        }
    }

    int32 OutPlayer = 0;
    int32 LeftPlayer = 0;

    for (TActorIterator<ABGPlayerController> It(GetWorld()); It; ++It)
    {
        ABGPlayerState* OtherPlayerState = It->GetPlayerState<ABGPlayerState>();
        if (!IsValid(CustomPlayerState)) continue;

        if (CustomPlayerState->PlayRoomID == OtherPlayerState->PlayRoomID)
        {
            LeftPlayer++;
            if (OtherPlayerState->IsOnGaming)
            {
                if (OtherPlayerState->PlayerTryAmount >= OtherPlayerState->MaxTryAmount)
                {
                    OutPlayer++;
                    It->ClientRPCTakeBlockMessage();
                }
            }

            FString MakePlayerName;
            if (*It == Player)
            {
                MakePlayerName = FString::Printf(TEXT("[ %8s ] : "), TEXT("Me"));
            }
            else
            {
                MakePlayerName = FString::Printf(TEXT("[ %8s ] : "), *CustomPlayerState->GetPlayerName());
            }
            FString CombineMessage = MakePlayerName + FString::Printf(TEXT("%-15s"), *InChatMessage) + JudgeResult;

            It->ClientRPCTakeChatMessage(CombineMessage);
        }

    }

    if (CustomPlayerState->IsOnGaming && GetResultData.Strike >= 3)
    {
        ABGGameStateBase* CustomGameState = GetGameState<ABGGameStateBase>();
        if (!IsValid(CustomGameState)) return;
        CustomGameState->BroadCastGameResultToInPlayer(FString::Printf(TEXT("Room%d Winner: %s"), CustomPlayerState->PlayRoomID, *CustomPlayerState->GetPlayerName()));

        for (TActorIterator<ABGPlayerController> It(GetWorld()); It; ++It)
        {
            It->ClientRPCTakeReleaseMessage();
            ABGPlayerState* OtherPlayerState = It->GetPlayerState<ABGPlayerState>();
            if (!IsValid(CustomPlayerState)) continue;

            if (CustomPlayerState->PlayRoomID == OtherPlayerState->PlayRoomID && *It != Player)
            {
                OtherPlayerState->PlayRoomID = 0;
                OtherPlayerState->PlayerTryAmount = 0;
                OtherPlayerState->IsOnGaming = false;

            }
        }

        RandomNumber.Remove(CustomPlayerState->PlayRoomID);
        CustomPlayerState->PlayRoomID = 0;
        CustomPlayerState->PlayerTryAmount = 0;
        CustomPlayerState->IsOnGaming = false;
    }
    else if (CustomPlayerState->IsOnGaming && LeftPlayer == OutPlayer)
    {
        ABGGameStateBase* CustomGameState = GetGameState<ABGGameStateBase>();
        if (!IsValid(CustomGameState)) return;
        CustomGameState->BroadCastGameResultToInPlayer(FString::Printf(TEXT("Room%d Draw"), CustomPlayerState->PlayRoomID));

        for (TActorIterator<ABGPlayerController> It(GetWorld()); It; ++It)
        {
            ABGPlayerState* OtherPlayerState = It->GetPlayerState<ABGPlayerState>();
            if (!IsValid(CustomPlayerState)) continue;

            if ((CustomPlayerState->PlayRoomID == OtherPlayerState->PlayRoomID) && (*It != Player))
            {
                It->ClientRPCTakeReleaseMessage();
                OtherPlayerState->PlayRoomID = 0;
                OtherPlayerState->PlayerTryAmount = 0;
                OtherPlayerState->IsOnGaming = false;
            }
        }

        Player->ClientRPCTakeReleaseMessage();
        CustomPlayerState->PlayRoomID = 0;
        CustomPlayerState->PlayerTryAmount = 0;
        CustomPlayerState->IsOnGaming = false;
    }
}
