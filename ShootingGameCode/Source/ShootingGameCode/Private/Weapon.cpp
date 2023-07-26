// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//WeaponMesh �� StaticMeshComponent�� ���� (Mesh��� �̸�����)
	//subobjcet�� weapon�� ��ӹ��� ���͸� �� �� �� ���� 
	//��� ���� �������Ʈ�� �ڵ� ����
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	

	WeaponMesh->SetCollisionProfileName("OverlapAllDynamic");
	//WeaponMesh�� �������ô��̳������� �ݸ��� ����

	bReplicates = true;
	SetReplicateMovement(true);

	SetRootComponent(WeaponMesh);
	//WeaponMesh�� ��Ʈ ������Ʈ

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
	if (IsValid(ShootMontage) == false) // ��ȿ�� �˻�
	{
		return;
	}

	OwnChar->PlayAnimMontage(ShootMontage);
	//ĳ���� ���۷��� OwnChar�� ShootMontage�� PlayAnimMontage
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(ReloadMontage) == false) // ��ȿ�� �˻�
	{
		return;
	}
	// !IsValid(ReloadMontage)


	OwnChar->PlayAnimMontage(ReloadMontage);
	//ĳ���� ���۷��� OwnChar�� ReloadMontage�� PlayAnimMontage
}


void AWeapon::EventShoot_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	//EventShoot �Լ��� ����Ǿ����� : 
	// ShootEffect�� WeaponMesh�� "Muzzle" ������ ��ġ�� ȸ�������� ����


	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound,
		WeaponMesh->GetSocketLocation("Muzzle"));
	// ShootSound�� WeaponMesh�� "Muzzle" ������ ��ġ���� ����
}

void AWeapon::EventPickUp_Implementation(ACharacter* targetChar)
{
	//ĳ���� ���۷��� ������ Ÿ�� ĳ����
	OwnChar = targetChar;

	//WeaponMesh�� �ù��� ��
	WeaponMesh->SetSimulatePhysics(false);

	AttachToComponent(targetChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

void AWeapon::EventDrop_Implementation(ACharacter* targetChar)
{
	OwnChar = nullptr;

	WeaponMesh->SetSimulatePhysics(true);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
