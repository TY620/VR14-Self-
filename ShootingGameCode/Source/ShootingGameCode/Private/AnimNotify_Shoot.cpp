// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Shoot.h"
#include "ShootingGameCodeCharacter.h" // 
#include "GameFramework/Character.h"
#include "WeaponInterface.h"

void UAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//pChar는 ShootingGameCodeCharacter 형변환 (메쉬컴포넌트는 캐릭터임(owner))
	AShootingGameCodeCharacter* pChar = Cast <AShootingGameCodeCharacter> (MeshComp->GetOwner());
	if (IsValid(pChar) == false)
	{
		return;
	}

	//InterfaceObj는 WeaponInterface 형변환 (ShootingGameCodeCharacter의 Weapon 레퍼런스인 EquipWeapon)
	IWeaponInterface* InterfaceObj = Cast <IWeaponInterface> (pChar->EquipWeapon);
	if (InterfaceObj == nullptr)
	{
		return;
	}
	//유효성 검사

	//인터페이스의 EventShoot 실행
	InterfaceObj->Execute_EventShoot(pChar->EquipWeapon);
}
