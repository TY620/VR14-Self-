// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//WeaponMesh 는 StaticMeshComponent를 생성 (Mesh라는 이름으로)
	//subobjcet는 weapon을 상속받은 액터를 다 쓸 수 있음 
	//상속 받은 블루프린트는 자동 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	

	WeaponMesh->SetCollisionProfileName("OverlapAllDynamic");
	//WeaponMesh는 오버랩올다이나믹으로 콜리전 설정

	bReplicates = true;
	SetReplicateMovement(true);

	SetRootComponent(WeaponMesh);
	//WeaponMesh는 루트 컴포넌트

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
	//캐릭터 레퍼런스 OwnChar는 ShootMontage를 PlayAnimMontage
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(ReloadMontage) == false) // 유효성 검사
	{
		return;
	}
	// !IsValid(ReloadMontage)


	OwnChar->PlayAnimMontage(ReloadMontage);
	//캐릭터 레퍼런스 OwnChar는 ReloadMontage를 PlayAnimMontage
}


void AWeapon::EventShoot_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	//EventShoot 함수가 실행되었을때 : 
	// ShootEffect를 WeaponMesh의 "Muzzle" 소켓의 위치와 회전값에서 구현


	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound,
		WeaponMesh->GetSocketLocation("Muzzle"));
	// ShootSound를 WeaponMesh의 "Muzzle" 소켓의 위치에서 구현
}

void AWeapon::EventPickUp_Implementation(ACharacter* targetChar)
{
	//캐릭터 레퍼런스 변수는 타겟 캐릭터
	OwnChar = targetChar;

	//WeaponMesh를 시뮬을 끔
	WeaponMesh->SetSimulatePhysics(false);

	AttachToComponent(targetChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

void AWeapon::EventDrop_Implementation(ACharacter* targetChar)
{
	OwnChar = nullptr;

	WeaponMesh->SetSimulatePhysics(true);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
