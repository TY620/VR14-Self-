// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstGun.h"
#include "GameFramework/Character.h"

void ABurstGun::EventTrigger_Implementation(bool IsPress)
{
	if (IsPress == true)
	{
		BurstCnt = 3;
		LoopShootMontage();
	}
}

void ABurstGun::LoopShootMontage()
{
	if (IsValid(weaponData->ShootMontage) == false)
	{
		return;
	}

	OwnChar->PlayAnimMontage(weaponData->ShootMontage);

	BurstCnt--;
	if (BurstCnt > 0)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.SetTimer(th_LoopShoot, this, &ABurstGun::LoopShootMontage, 0.1f, false);
	}
}
