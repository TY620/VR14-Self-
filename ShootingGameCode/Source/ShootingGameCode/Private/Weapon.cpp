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

	//WeaponMesh 는 StaticMeshComponent를 생성 (Mesh라는 이름으로)
	//subobjcet는 weapon을 상속받은 액터를 다 쓸 수 있음 
	//상속 받은 블루프린트는 자동 생성
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	//WeaponMesh는 weapon이라는 콜리전 프리셋으로 콜리전 설정
	WeaponMesh->SetCollisionProfileName("weapon");
	//WeaponMesh에 물리 시뮬을 활성화
	WeaponMesh->SetSimulatePhysics(true);

	//네트워크에서 복제 가능하도록(서버,클라)
	bReplicates = true;
	//움직임이 네트워크에서 복제 가능하도록
	SetReplicateMovement(true);

	//WeaponMesh는 루트 컴포넌트
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

	//캐릭터 레퍼런스 OwnChar는 ShootMontage를 PlayAnimMontage
	OwnChar->PlayAnimMontage(weaponData->ShootMontage);
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(weaponData->ReloadMontage) == false)
	{
		return;
	}
	// !IsValid(ReloadMontage);

	//캐릭터 레퍼런스 OwnChar는 ReloadMontage를 PlayAnimMontage
	OwnChar->PlayAnimMontage(weaponData->ReloadMontage);
}


void AWeapon::EventShoot_Implementation()
{
	if(IsCanShoot() == false)
	{
		return;
	}

	//EventShoot 함수가 실행되었을때 : ShootEffect를 WeaponMesh의 "Muzzle" 소켓의 위치와 회전값에서 구현
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), weaponData->FireEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	// EventShoot 함수가 실행되었을때 : ShootSound를 WeaponMesh의 "Muzzle" 소켓의 위치에서 구현
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), weaponData->SoundBase,
		WeaponMesh->GetSocketLocation("Muzzle"));

	//Aplayercontroller헤더 include
	APlayerController* Shooter = GetWorld()->GetFirstPlayerController();

	//카메라의 위치는 플레이어 컨트롤러의 카메라의 카메라 위치
	FVector CameraLocation = Shooter->PlayerCameraManager->GetCameraLocation();
	// 카메라 ForwardVector는 플레이어 컨트롤러의 카메라의 ForwardVector
	FVector CameraForwardVector = Shooter->PlayerCameraManager->GetActorForwardVector();
	// 카메라의 ForwardVector * 카메라의 발사 시작 길이 함수+플레이어 컨트롤러 카메라 위치가 Start
	FVector Start = (CameraForwardVector * GetFireStartLength()) + CameraLocation;
	// 카메라의 ForwardVector * 5000 + 플레이어 컨트롤러 카메라의 위치가 End
	FVector End = (CameraForwardVector * 5000.0f) + CameraLocation;

	ReqShoot(Start, End);
}

void AWeapon::EventPickUp_Implementation(ACharacter* targetChar)
{
	//캐릭터 레퍼런스 변수는 타겟 캐릭터
	OwnChar = targetChar;

	//WeaponMesh를 시뮬을 끔
	WeaponMesh->SetSimulatePhysics(false);

	//weapon메시를 targetChar(캐릭터 레퍼런스)의 메시에 고정된 위치로 어태치함
	AttachToComponent(targetChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));

	// 총을 줍고 나서 Ammo 세팅
	UpdateAmmoToHUD(Ammo);
}

void AWeapon::EventDrop_Implementation(ACharacter* targetChar)
{
	//총 버리기 전에 0으로 세팅하고 디태치 진행
	UpdateAmmoToHUD(0);
	
	//캐릭터에게 소유되지 않음
	OwnChar = nullptr;

	//WeaponMesh를 시뮬을 킴
	WeaponMesh->SetSimulatePhysics(true);

	//현재 액터에서 액터를 분리시킴(회전과 이동 변화 없이)
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
	// 추적할 오브젝트 타입
	FCollisionObjectQueryParams CollisionObjectQuery;
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	CollisionObjectQuery.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
	//예외설정만 해서는 실행이 안됨, 충돌 오브젝트 타입 다 설정해야 함.

	// 예외 설정
	FCollisionQueryParams CollisionQuery;
	CollisionQuery.AddIgnoredActor(OwnChar);

	//Line Trace오브젝트
	//Line Trace 검사 bool 변수
	bool isHit = GetWorld()->LineTraceSingleByObjectType(Result, vStart, vEnd, CollisionObjectQuery);
	//디버그 라인 세팅
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
	// 데미지 받는 액터, 데미지, 데미지 주는 컨트롤러, 데미지의 원인(AWeapon), 데미지 타입
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
	//스프링 암 컴포넌트사용하기 위해 헤더 include
	// Arm은 USpringArmComponent Ownchar의 스프링암 컴포넌트 형변환
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
	//캐릭터가 총을 들고 있는지 확인
	if(IsValid(OwnChar) == false)
	{
		return;
	}
	
	//pPlayer0은 해당 플레이어 컨트롤러
	APlayerController* pPlayer0 = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//pOwnController는 현재 캐릭터의 컨트롤러
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
