// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorSpawner.h"
#include "ShootingGameInstance.h"
#include "Weapon.h"

// Sets default values
AActorSpawner::AActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnRandomWeapon();

	//서버인지 확인
	if (HasAuthority())
	{
		//월드에 AWeapon클래스(spawnclass에 Weapon을 넣어줌)를 액터 위치(actorspawner)로 액터 생성
		GetWorld()->SpawnActor<AWeapon>(SpawnClass, GetActorTransform());
	}
	
}

// Called every frame
void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActorSpawner::SpawnRandomWeapon()
{
	if (HasAuthority())
	{
		//gameInst는 UShootingGameInstance로 형변환
		UShootingGameInstance* gameInst = Cast<UShootingGameInstance>(GetGameInstance());
		//rowName은 UShootingGameInstance의 랜덤 행이름 함수
		FName rowName = gameInst->GetWeaponRandomRowName();
		//data는 UShootingGameInstance의 
		FST_Weapon* data = gameInst->GetWeaponRowData(rowName);

		AWeapon* weapon = GetWorld()->SpawnActor<AWeapon>(data->WeaponClass, GetActorTransform());
		if (weapon)
		{ 
			weapon->SetWeaponRowName(rowName);
		}
	}
}

