// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"



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

	// EventShoot �Լ��� ����Ǿ����� : ShootSound�� WeaponMesh�� "Muzzle" ������ ��ġ���� ����
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound,
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

void AWeapon::IsCanPickUp_Implementation(bool& IsCanPickUp)
{
	if (OwnChar != nullptr)
	{
		IsCanPickUp = false;
		return;
	}
	
	IsCanPickUp = true;
	
}

void AWeapon::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
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
	GetWorld()->LineTraceSingleByObjectType(Result, vStart, vEnd, CollisionObjectQuery);
	//����� ���� ����
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Yellow, true);
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
