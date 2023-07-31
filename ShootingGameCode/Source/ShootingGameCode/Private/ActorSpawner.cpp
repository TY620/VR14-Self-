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

	//�������� Ȯ��
	if (HasAuthority())
	{
		//���忡 AWeaponŬ����(spawnclass�� Weapon�� �־���)�� ���� ��ġ(actorspawner)�� ���� ����
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
		//gameInst�� UShootingGameInstance�� ����ȯ
		UShootingGameInstance* gameInst = Cast<UShootingGameInstance>(GetGameInstance());
		//rowName�� UShootingGameInstance�� ���� ���̸� �Լ�
		FName rowName = gameInst->GetWeaponRandomRowName();
		//data�� UShootingGameInstance�� 
		FST_Weapon* data = gameInst->GetWeaponRowData(rowName);

		AWeapon* weapon = GetWorld()->SpawnActor<AWeapon>(data->WeaponClass, GetActorTransform());
		if (weapon)
		{ 
			weapon->SetWeaponRowName(rowName);
		}
	}
}

