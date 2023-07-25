// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//subobjcet는 weapon을 상속받은 액터를 다 쓸 수 있음
	WeaponMesh->SetCollisionProfileName("OverlapAllDynamic");

	SetRootComponent(WeaponMesh);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventTrigger_Implementation()
{
	if (IsValid(ShootMontage) == false) // 유효성 검사
	{
		return;
	}

	OwnChar->PlayAnimMontage(ShootMontage);
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(ReloadMontage) == false) // 유효성 검사
	{
		return;
	}

	OwnChar->PlayAnimMontage(ReloadMontage);
}


void AWeapon::EventShoot_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound,
		WeaponMesh->GetSocketLocation("Muzzle"));
}