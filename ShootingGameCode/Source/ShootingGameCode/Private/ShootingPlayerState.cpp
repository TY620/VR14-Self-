// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerState.h"
//���ø�����Ʈ�� ���� ��Ʈ��ũ ��� �߰�
#include "Net/UnrealNetwork.h"

AShootingPlayerState::AShootingPlayerState()
{
	CurHP = 100;
	MaxHP = 100;
	Mag = 0;
}

void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	//���ø�����Ʈ �ϱ� ���ؼ�
	DOREPLIFETIME(AShootingPlayerState, CurHP);
	DOREPLIFETIME(AShootingPlayerState, MaxHP);
	DOREPLIFETIME(AShootingPlayerState, Mag);

}

void AShootingPlayerState::OnRep_CurHP()
{
	//�̺�Ʈ ������ ȣ��
	if (Fuc_Dele_UpdataeHP.IsBound())
	{
		Fuc_Dele_UpdataeHP.Broadcast(CurHP, MaxHP);
	}
}

void AShootingPlayerState::OnRep_MaxHP()
{
}

void AShootingPlayerState::OnRep_Mag()
{
	if (Fuc_Dele_UpdateMag.IsBound())
	{
		Fuc_Dele_UpdateMag.Broadcast(Mag);
	}

}

void AShootingPlayerState::AddDamage(float Damage)
{
	CurHP = CurHP - Damage;

	OnRep_CurHP();
}

void AShootingPlayerState::AddMag()
{
	Mag = Mag + 1;
	OnRep_Mag();
}

bool AShootingPlayerState::UseMag()
{
	if (IsCanUseMag() == false)
	{
		return false;
	}
	
	Mag = Mag - 1;
	OnRep_Mag();

	return true;
}

bool AShootingPlayerState::IsCanUseMag()
{
	if (Mag <= 0)
	{
		return false;
	}
	return true;

	// return (Mag >0) ? true : false;
}
