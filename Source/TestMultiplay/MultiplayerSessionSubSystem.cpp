#include "MultiplayerSessionSubSystem.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"



UMultiplayerSessionSubSystem::UMultiplayerSessionSubSystem()
{
//	PrintString("MSS Constructor");
	CreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("Co-op Adventure Session Name");
}

void UMultiplayerSessionSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
//	PrintString("MSS Initialize");
	IOnlineSubsystem* OnlineSubsytem =IOnlineSubsystem::Get();
	if (OnlineSubsytem)
	{
		FString SubSystemName = OnlineSubsytem->GetSubsystemName().ToString();
		PrintString(SubSystemName);

		 SessionInterface = OnlineSubsytem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this,&UMultiplayerSessionSubSystem::OnCreateSessionComplete );
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionSubSystem::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionSubSystem::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionSubSystem::OnJoinSessionComplete);
		}
	}
}

void UMultiplayerSessionSubSystem::Deinitialize()
{
//	UE_LOG(LogTemp, Warning, TEXT("MSS DeInitialize"));
}

void UMultiplayerSessionSubSystem::PrintString(const FString& arr)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan, arr);
	}
}

void UMultiplayerSessionSubSystem::CreateServer(FString ServerName)
{
	PrintString("Create Server");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!");
		ServerCreateDel.Broadcast(false);
		return;
	}

	FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession)
	{
		FString Mssg = FString::Printf(TEXT("Session with name %s already exists, destroying it"), *MySessionName.ToString());
		PrintString(Mssg);
		CreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}

	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	bool IsLan = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLan = true;
	}
	SessionSettings.bIsLANMatch = IsLan;

	SessionSettings.Set(FName("SERVER_NAME"),ServerName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);

}

void UMultiplayerSessionSubSystem::FindServer(FString ServerName)
{
	PrintString("Find Server");

	if (ServerName.IsEmpty())
	{
		PrintString("Server name cannot be empty!");
		ServerJoinDel.Broadcast(false);
		return;
	}
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	bool IsLAN = false;

	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}

	SessionSearch->bIsLanQuery = IsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

}

void UMultiplayerSessionSubSystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %d"), WasSuccessful));

	ServerCreateDel.Broadcast(WasSuccessful);

	if (WasSuccessful)
	{
		FString Path = "/Game/ThirdPerson/Maps/ThirdPersonMap?listen";
		if (!GameMapPath.IsEmpty())
		{
			Path = FString::Printf(TEXT("%s?listen"), *GameMapPath);
		}
			GetWorld()->ServerTravel(Path);
	}
}

void UMultiplayerSessionSubSystem::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
	FString Mssg = FString::Printf(TEXT("OnDestroySessionComplete,SessionName : %s, Success : %d"), *SessionName.ToString(), WasSuccessful);
	PrintString(Mssg);

	if (CreateServerAfterDestroy)
	{
		CreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerSessionSubSystem::OnFindSessionComplete(bool WasSuccessful)
{
	if (!WasSuccessful) return;
	if (ServerNameToFind.IsEmpty()) return;

	TArray<FOnlineSessionSearchResult> Result = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* CorrectReult = 0;

	if (Result.Num() > 0)
	{
		FString Mssg = FString::Printf(TEXT("%d Sessions found"), Result.Num());
		PrintString(Mssg);

		for (FOnlineSessionSearchResult a : Result)
		{
			if (a.IsValid())
			{
				FString ServerName = "No_name";
				a.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

				if (ServerName.Equals(ServerNameToFind))
				{
					CorrectReult = &a;
					FString Mssg2 = FString::Printf(TEXT("Found Server name : %s"), *ServerName);
					PrintString(Mssg2);
					break;
				}
			}
		}
		if (CorrectReult)
		{
			SessionInterface->JoinSession(0, MySessionName, *CorrectReult);
		}
		else
		{
			PrintString(FString::Printf(TEXT("Couldn't find Server : %s"), *ServerNameToFind));
			ServerNameToFind = "";
			ServerJoinDel.Broadcast(false);
		}
	}
	else
	{
		PrintString("Zero Sessions found");
			ServerJoinDel.Broadcast(false);
	}

}

void UMultiplayerSessionSubSystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	ServerJoinDel.Broadcast(Result == EOnJoinSessionCompleteResult::Success);

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString Mssg = FString::Printf(TEXT("Successfully joined session %s"),*SessionName.ToString());
		PrintString(Mssg);
		
		FString Adress = "";
		bool Success = SessionInterface->GetResolvedConnectString(MySessionName, Adress);
		if (Success)
		{
			PrintString(FString::Printf(TEXT("Adress: %s"), *Adress));
			APlayerController *PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

			if (PlayerController)
			{
				PlayerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
			}
		}
		else
		{
			PrintString("GetResolvedConnectString reutrned false!");			
		}
	}
	else
	{
		PrintString("OnJoinSessionComplete failed");
	}

}
