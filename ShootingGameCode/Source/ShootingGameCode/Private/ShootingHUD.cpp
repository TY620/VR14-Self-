// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingHUD.h"
//���� ���� ���� ���� include
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
	//pc�� �÷��̾� ��Ʈ�ѷ�
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	if (IsValid(pc))
	{
		//ps�� ���� �÷��̾� ��Ʈ�ѷ��� ����ȯ
		AShootingPlayerState* ps = Cast<AShootingPlayerState>(pc->PlayerState);
		
		if (IsValid(ps))
		{
			//���ε�
			ps->Fuc_Dele_UpdataeHP.AddDynamic(this, &AShootingHUD::OnUpdateMyHP);
			//�ʱ� �� ����
			OnUpdateMyHP(ps->CurHP, ps->MaxHP);

			ps->Fuc_Dele_UpdateMag.AddDynamic(this, &AShootingHUD::OnUpdateMyMag);
			return;

		}
	}

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	//0.1�� �ڿ� �ݺ����� �ʰ� BindMyPlayerState�� ȣ���� ShootingHUD��
	timerManager.SetTimer(th_BindMyPlayerState, this, &AShootingHUD::BindMyPlayerState, 0.1f, false);
	//��� �Լ��� �������
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
