// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootingHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API AShootingHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	void BindMyPlayerState();

	//BlueprintNativeEvent : 블루프린트에서 재정의 // 블루프린트로  설정
	//블루프린트에서 함수 오버라이드 가능
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyHP(float CurHP, float MaxHP);

	void OnUpdateMyHP_Implementation(float CurHP, float MaxHP);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateMyAmmo(int Ammo);

	void OnUpdateMyAmmo_Implementation(int Ammo);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HUDWidget;
	
	//타이머 핸들 
	FTimerHandle th_BindMyPlayerState;
};
