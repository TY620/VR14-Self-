// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Shoot.h"
#include "ShootingGameCodeCharacter.h" // 
#include "GameFramework/Character.h"
#include "WeaponInterface.h"

void UAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//pChar�� ShootingGameCodeCharacter ����ȯ (�޽�������Ʈ�� ĳ������(owner))
	AShootingGameCodeCharacter* pChar = Cast <AShootingGameCodeCharacter> (MeshComp->GetOwner());
	if (IsValid(pChar) == false)
	{
		return;
	}

	//InterfaceObj�� WeaponInterface ����ȯ (ShootingGameCodeCharacter�� Weapon ���۷����� EquipWeapon)
	IWeaponInterface* InterfaceObj = Cast <IWeaponInterface> (pChar->EquipWeapon);
	if (InterfaceObj == nullptr)
	{
		return;
	}
	//��ȿ�� �˻�

	//�������̽��� EventShoot ����
	InterfaceObj->Execute_EventShoot(pChar->EquipWeapon);
}
