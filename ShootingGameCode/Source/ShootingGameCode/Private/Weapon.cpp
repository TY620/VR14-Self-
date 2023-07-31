// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "ShootingGameInstance.h"
#include "ShootingHUD.h"


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
	
	UShootingGameInstance* gameInst = Cast<UShootingGameInstance>(GetGameInstance());
	weaponData = gameInst->GetWeaponRowData(RowName);

	if (weaponData != nullptr)
	{
		SetWeaponData(RowName);
	}

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, Ammo);
	DOREPLIFETIME(AWeapon, RowName);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventTrigger_Implementation(bool IsPress)
{
	if (IsValid(weaponData->ShootMontage) == false)
	{
		return;
	}

	//ĳ���� ���۷��� OwnChar�� ShootMontage�� PlayAnimMontage
	OwnChar->PlayAnimMontage(weaponData->ShootMontage);
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(weaponData->ReloadMontage) == false)
	{
		return;
	}
	// !IsValid(ReloadMontage);

	//ĳ���� ���۷��� OwnChar�� ReloadMontage�� PlayAnimMontage
	OwnChar->PlayAnimMontage(weaponData->ReloadMontage);
}


void AWeapon::EventShoot_Implementation()
{
	if(IsCanShoot() == false)
	{
		return;
	}

	//EventShoot �Լ��� ����Ǿ����� : ShootEffect�� WeaponMesh�� "Muzzle" ������ ��ġ�� ȸ�������� ����
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), weaponData->FireEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	// EventShoot �Լ��� ����Ǿ����� : ShootSound�� WeaponMesh�� "Muzzle" ������ ��ġ���� ����
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), weaponData->SoundBase,
		WeaponMesh->GetSocketLocation("Muzzle"));

	//Aplayercontroller��� include
	APlayerController* Shooter = GetWorld()->GetFirstPlayerController();

	//ī�޶��� ��ġ�� �÷��̾� ��Ʈ�ѷ��� ī�޶��� ī�޶� ��ġ
	FVector CameraLocation = Shooter->PlayerCameraManager->GetCameraLocation();
	// ī�޶� ForwardVector�� �÷��̾� ��Ʈ�ѷ��� ī�޶��� ForwardVector
	FVector CameraForwardVector = Shooter->PlayerCameraManager->GetActorForwardVector();
	// ī�޶��� ForwardVector * ī�޶��� �߻� ���� ���� �Լ�+�÷��̾� ��Ʈ�ѷ� ī�޶� ��ġ�� Start
	FVector Start = (CameraForwardVector * GetFireStartLength()) + CameraLocation;
	// ī�޶��� ForwardVector * 5000 + �÷��̾� ��Ʈ�ѷ� ī�޶��� ��ġ�� End
	FVector End = (CameraForwardVector * 5000.0f) + CameraLocation;

	ReqShoot(Start, End);
}

void AWeapon::EventPickUp_Implementation(ACharacter* targetChar)
{
	//ĳ���� ���۷��� ������ Ÿ�� ĳ����
	OwnChar = targetChar;

	//WeaponMesh�� �ù��� ��
	WeaponMesh->SetSimulatePhysics(false);

	//weapon�޽ø� targetChar(ĳ���� ���۷���)�� �޽ÿ� ������ ��ġ�� ����ġ��
	AttachToComponent(targetChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

	// ���� �ݰ� ���� Ammo ����
	UpdateAmmoToHUD(Ammo);
}

void AWeapon::EventDrop_Implementation(ACharacter* targetChar)
{
	//�� ������ ���� 0���� �����ϰ� ����ġ ����
	UpdateAmmoToHUD(0);
	
	//ĳ���Ϳ��� �������� ����
	OwnChar = nullptr;

	//WeaponMesh�� �ù��� Ŵ
	WeaponMesh->SetSimulatePhysics(true);

	//���� ���Ϳ��� ���͸� �и���Ŵ(ȸ���� �̵� ��ȭ ����)
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeapon::IsCanPickUp_Implementation(bool& IsCanPickUp)
{
	if (OwnChar != nullptr)
	{
		IsCanPickUp = false;
		return;
	}
	
	IsCanPickUp = true;
	
}

void AWeapon::EventResetAmmo_Implementation()
{
	SetAmmo(weaponData->MaxAmmo);
}

void AWeapon::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
	if(UseAmmo() == false)
	{
		return;
	}
	
	FHitResult Result;
	// ������ ������Ʈ Ÿ��
	FCollisionObjectQueryParams CollisionObjectQuery;
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
	//���ܼ����� �ؼ��� ������ �ȵ�, �浹 ������Ʈ Ÿ�� �� �����ؾ� ��.

	// ���� ����
	FCollisionQueryParams CollisionQuery;
	CollisionQuery.AddIgnoredActor(OwnChar);

	//Line Trace������Ʈ
	//Line Trace �˻� bool ����
	bool isHit = GetWorld()->LineTraceSingleByObjectType(Result, vStart, vEnd, CollisionObjectQuery);
	//����� ���� ����
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Yellow, true);

	if(isHit == false)
	{
		return;
	}

	ACharacter* HitChar = Cast<ACharacter>(Result.GetActor());
	
	if (HitChar == nullptr)
	{
		return;
	}
	// ������ �޴� ����, ������, ������ �ִ� ��Ʈ�ѷ�, �������� ����(AWeapon), ������ Ÿ��
	UGameplayStatics::ApplyDamage(HitChar, weaponData->Damage, OwnChar->GetController
		(), this, UDamageType::StaticClass());
}

void AWeapon::OnRep_Ammo()
{
	UpdateAmmoToHUD(Ammo);
}

void AWeapon::OnRep_RowName()
{
	SetWeaponData(RowName);
}

bool AWeapon::IsCanShoot() const
{
	if (Ammo <= 0)
	{
		return false;
	}

	return true;
}

float AWeapon::GetFireStartLength()
{
	if (IsValid(OwnChar) == false)
	{
		return 0.0f;
	}
	//������ �� ������Ʈ����ϱ� ���� ��� include
	// Arm�� USpringArmComponent Ownchar�� �������� ������Ʈ ����ȯ
	USpringArmComponent* Arm = Cast<USpringArmComponent>(OwnChar->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (IsValid(Arm) == false)
	{
		return 0.0f;
	}

	return Arm->TargetArmLength + 100;
	
}

bool AWeapon::UseAmmo()
{
	if (IsCanShoot() == false)
	{
		return false;
	}

	Ammo = Ammo - 1;
	OnRep_Ammo();
	return true;
}

void AWeapon::UpdateAmmoToHUD(int NewAmmo)
{
	//ĳ���Ͱ� ���� ��� �ִ��� Ȯ��
	if(IsValid(OwnChar) == false)
	{
		return;
	}
	
	//pPlayer0�� �ش� �÷��̾� ��Ʈ�ѷ�
	APlayerController* pPlayer0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//pOwnController�� ���� ĳ������ ��Ʈ�ѷ�
	AController* pOwnController = OwnChar->GetController();

	if (pPlayer0 != pOwnController)
	{
		return;
	}

	AShootingHUD* pHUD = Cast<AShootingHUD>(pPlayer0->GetHUD());
	if (IsValid(pHUD) == false)
	{
		return;
	}

	pHUD->OnUpdateMyAmmo(Ammo);
}

void AWeapon::SetAmmo(int NewAmmo)
{
	Ammo = NewAmmo;
	OnRep_Ammo();
}

void AWeapon::SetWeaponData(FName name)
{
	UShootingGameInstance* gameInst = Cast<UShootingGameInstance>(GetGameInstance());
	weaponData = gameInst->GetWeaponRowData(name);

	WeaponMesh->SetStaticMesh(weaponData->StaticMesh);
	EventResetAmmo();
}

void AWeapon::SetWeaponRowName(FName name)
{
	RowName = name;
	OnRep_RowName();
}
