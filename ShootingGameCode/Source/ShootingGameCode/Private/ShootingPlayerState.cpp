// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerState.h"
//���ø�����Ʈ�� ���� ��Ʈ��ũ ��� �߰�
#include "Net/UnrealNetwork.h"

void AShootingPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShootingPlayerState, CurHP);
	DOREPLIFETIME(AShootingPlayerState, MaxHP);
}

AShootingPlayerState::AShootingPlayerState()
{
	CurHP = 100;
	MaxHP = 100;
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

void AShootingPlayerState::AddDamage(float Damage)
{
	CurHP = CurHP - Damage;

	OnRep_CurHP();
}
