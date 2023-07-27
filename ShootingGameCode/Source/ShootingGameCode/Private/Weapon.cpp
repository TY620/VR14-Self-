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

	//캐릭터 레퍼런스 OwnChar는 ShootMontage를 PlayAnimMontage
	OwnChar->PlayAnimMontage(ShootMontage);
}

void AWeapon::EventReload_Implementation()
{
	if (IsValid(ReloadMontage) == false)
	{
		return;
	}
	// !IsValid(ReloadMontage);

	//캐릭터 레퍼런스 OwnChar는 ReloadMontage를 PlayAnimMontage
	OwnChar->PlayAnimMontage(ReloadMontage);
}


void AWeapon::EventShoot_Implementation()
{
	//EventShoot 함수가 실행되었을때 : ShootEffect를 WeaponMesh의 "Muzzle" 소켓의 위치와 회전값에서 구현
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect,
		WeaponMesh->GetSocketLocation("Muzzle"),
		WeaponMesh->GetSocketRotation("Muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	// EventShoot 함수가 실행되었을때 : ShootSound를 WeaponMesh의 "Muzzle" 소켓의 위치에서 구현
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootSound,
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
}

void AWeapon::EventDrop_Implementation(ACharacter* targetChar)
{
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

void AWeapon::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
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
	GetWorld()->LineTraceSingleByObjectType(Result, vStart, vEnd, CollisionObjectQuery);
	//디버그 라인 세팅
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Yellow, true);
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
