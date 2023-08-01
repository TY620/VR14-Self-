// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingHUD.h"
//유저 위젯 쓰기 위한 include
#include "Blueprint/UserWidget.h"
#include "ShootingPlayerState.h"

void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();

	BindMyPlayerState();
}

void AShootingHUD::BindMyPlayerState()
{
	//pc는 플레이어 컨트롤러
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	if (IsValid(pc))
	{
		//ps는 현재 플레이어 컨트롤러로 형변환
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(pc->PlayerState);
		
		if (IsValid(ps))
		{
			//바인딩
			ps->Fuc_Dele_UpdataeHP.AddDynamic(this, &AShootingHUD::OnUpdateMyHP);
			//초기 피 세팅
			OnUpdateMyHP(ps->CurHP, ps->MaxHP);

			ps->Fuc_Dele_UpdateMag.AddDynamic(this, &AShootingHUD::OnUpdateMyMag);
			return;

		}
	}

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	//0.1초 뒤에 반복하지 않고 BindMyPlayerState를 호출함 ShootingHUD가
	timerManager.SetTimer(th_BindMyPlayerState, this, &AShootingHUD::BindMyPlayerState, 0.1f, false);
	//재귀 함수로 만든거임
}

void AShootingHUD::OnUpdateMyHP_Implementation(float CurHP, float MaxHP)
{
}

void AShootingHUD::OnUpdateMyAmmo_Implementation(int Ammo)
{
}

void AShootingHUD::OnUpdateMyMag_Implementation(int Mag)
{
}
