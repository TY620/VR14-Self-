// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyHUD.h"
#include "LobbyPlayerState.h"

void ALobbyHUD::BindPlayerState(ALobbyPlayerState* PlayerState)
{
	if (IsValid(PlayerState))
	{
		PlayerState->Func_Dele_UpdateUserName.AddDynamic(this, &ALobbyHUD::OnUpdateUserName);
	}
}

void ALobbyHUD::OnUpdateUserName_Implementation(const FString& userName)
{
	GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Yellow,
		FString::Printf(TEXT("(Client)OnUpdateUserName userName=%s"), *userName));
}
