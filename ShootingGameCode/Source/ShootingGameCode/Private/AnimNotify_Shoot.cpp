// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Shoot.h"
#include "ShootingGameCodeCharacter.h" // 
#include "GameFramework/Character.h"
#include "WeaponInterface.h"

void UAnimNotify_Shoot::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AShootingGameCodeCharacter* pChar = Cast <AShootingGameCodeCharacter> (MeshComp->GetOwner());
	if (IsValid(pChar) == false)
	{
		return;
	}
	//pChar는 ShootingGameCodeCharacter 형변환 (메쉬컴포넌트는 캐릭터임(owner))
	//유효성 검사

	IWeaponInterface* InterfaceObj = Cast <IWeaponInterface> (pChar->EquipWeapon);
	if (InterfaceObj == nullptr)
	{
		return;
	}
	//InterfaceObj는 WeaponInterface 형변환
	// ㄴ(ShootingGameCodeCharacter의 Weapon 레퍼런스 EquipWeapon)
	//유효성 검사

	InterfaceObj->Execute_EventShoot(pChar->EquipWeapon);
	//인터페이스의 EventShoot 실행
}
