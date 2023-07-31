// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineGun.h"
#include "GameFramework/Character.h"


void AMachineGun::EventTrigger_Implementation(bool IsPress)
{
	if (IsPress)
	{
		LoopShootMontage();
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		//set timer by function name
		timerManager.SetTimer(th_LoopShoot, this, &AMachineGun::LoopShootMontage, 0.2f, true);
	}
	else
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(th_LoopShoot);
	}

}

void AMachineGun::LoopShootMontage()
{
	if (IsValid(weaponData->ShootMontage) == false)
	{
		return;
	}

	OwnChar->PlayAnimMontage(weaponData->ShootMontage);
}
