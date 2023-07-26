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
	
	//WeaponMesh�� weapon�̶�� �ݸ��� ���������� �ݸ��� ����
	WeaponMesh->SetCollisionProfileName("weapon");
	//WeaponMesh�� ���� �ù��� Ȱ��ȭ
	WeaponMesh->SetSimulatePhysics(true);

	//��Ʈ��ũ���� ���� �����ϵ���(����,Ŭ��)
	bReplicates = true;
	//�������� ��Ʈ��ũ���� ���� �����ϵ���
	SetReplicateMovement(true);

	//WeaponMesh�� ��Ʈ ������Ʈ
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
	if (IsValid(ShootMontage) == false)
	{
		return;
	}

	//ĳ���� ���۷��� OwnChar�� ShootMontage�� PlayAnimMontage
	OwnChar->PlayAnimMontage(ShootMontage);
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(ReloadMontage) == false)
	{
		return;
	}
	// !IsValid(ReloadMontage);

	//ĳ���� ���۷��� OwnChar�� ReloadMontage�� PlayAnimMontage
	OwnChar->PlayAnimMontage(ReloadMontage);
}


void AWeapon::EventShoot_Implementation()
{
	//EventShoot �Լ��� ����Ǿ����� : ShootEffect�� WeaponMesh�� "Muzzle" ������ ��ġ�� ȸ�������� ����
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	// ShootSound�� WeaponMesh�� "Muzzle" ������ ��ġ���� ����
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound,
		WeaponMesh->GetSocketLocation("Muzzle"));
}

void AWeapon::EventPickUp_Implementation(ACharacter* targetChar)
{
	//ĳ���� ���۷��� ������ Ÿ�� ĳ����
	OwnChar = targetChar;

	//WeaponMesh�� �ù��� ��
	WeaponMesh->SetSimulatePhysics(false);

	//weapon�޽ø� targetChar(ĳ���� ���۷���)�� �޽ÿ� ������ ��ġ�� ����ġ��
	AttachToComponent(targetChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

void AWeapon::EventDrop_Implementation(ACharacter* targetChar)
{
	//ĳ���Ϳ��� �������� ����
	OwnChar = nullptr;

	//WeaponMesh�� �ù��� Ŵ
	WeaponMesh->SetSimulatePhysics(true);

	//���� ���Ϳ��� ���͸� �и���Ŵ(ȸ���� �̵� ��ȭ ����)
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
