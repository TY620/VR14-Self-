// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingGameCodeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h" // 네트워크 관련 코딩 시 필수
#include "Public/Weapon.h"


//////////////////////////////////////////////////////////////////////////
// AShootingGameCodeCharacter

AShootingGameCodeCharacter::AShootingGameCodeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//
void AShootingGameCodeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// #include "Net/UnrealNetwork.h" 네트워크 관련 코딩 시 필수

	// 리플리케이트 관련 변수 사용 할 때 마다 여기에 추가 해야됨
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShootingGameCodeCharacter, PlayerRotation);
	DOREPLIFETIME(AShootingGameCodeCharacter, EquipWeapon);

	
	// PlayerRotation, EquipWeapon

}

void AShootingGameCodeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//
void AShootingGameCodeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // 부모 클래스 사용할 때 써주는 거(외워)

	if (HasAuthority() == true) // 서버가 맞으면
	{
		PlayerRotation = GetControlRotation(); // PlayerRotation은 컨트롤러(플레이어0)의 회전값
	}

}

void AShootingGameCodeCharacter::ReqPressF_Implementation()
{
	AActor* nearestWeapon = FindNearestWeapon();

	if (IsValid(nearestWeapon) == false)
	{
		return;
	}

	EquipWeapon = nearestWeapon;

	OnRep_EquipWeapon();
}

void AShootingGameCodeCharacter::OnRep_EquipWeapon()
{
	//컨트롤러 Yaw사용은 EquipWeapon 유효성에 따라 true, false
	bUseControllerRotationYaw = IsValid(EquipWeapon);
	
	//InterfaceObj는 WeaponInterface 형변환 (EquipWeapon을)
	//EquipWeapon이 WeaponInterface를 구현하는 경우
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(EquipWeapon);

	if (InterfaceObj == nullptr)
	{
		return;
	}

	// InterfaceObj가 IWeaponInterface를 구현하는 무기일 경우, 해당 무기의 EventPickUp 함수를 호출
	InterfaceObj->Execute_EventPickUp(EquipWeapon, this);
}

void AShootingGameCodeCharacter::ReqDrop_Implementation()
{
	ResDrop();
}

void AShootingGameCodeCharacter::ResDrop_Implementation()
{
	//InterfaceObj는 IWeaponInterface의 형변환 (Weapon 레퍼런스 EquipWeapon을)
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(EquipWeapon);

	if (InterfaceObj == nullptr)
	{
		return;
	}
	// if(!InterfaceObj)

	InterfaceObj->Execute_EventDrop(EquipWeapon, this);

	EquipWeapon = nullptr;
}

void AShootingGameCodeCharacter::EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass)
{
	// EquipWeapon은 월드(로테이션 0,0,0)(로테이터(0,0,0)에서 SpawnActor됨
	EquipWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));

	// pWeapon은 AWeapon의 형변환 (AWeapon의 레퍼런스 EquipWeapon을)
	// 만약 EquipWeapon이 AWeapon 혹은 자식이 아니라면 -> pWeapon = nullptr 이 됨
	// 들고 있는 액터가 무기가 맞는지에 대한 유효성 검사
	AWeapon* pWeapon = Cast<AWeapon>(EquipWeapon);
	
	
	// 플레이어 무기 장착에 대한 유효성 검사, 
	if (IsValid(pWeapon) == false)
	{
		return;
	}
	
	// pWeapon의 캐릭터 레퍼런스는 지금 플레이어
	pWeapon->OwnChar = this;
	

	// weapon의 메쉬를 컴포넌트로 어태치. getmesh가 부모 
	EquipWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon"));
}

AActor* AShootingGameCodeCharacter::FindNearestWeapon()
{
	TArray<AActor*> actors;
	GetCapsuleComponent()->GetOverlappingActors(actors, AWeapon::StaticClass());

	double nearestLength = 99999999.0f;
	AActor* nearestWeapon = nullptr;

	for (AActor* target : actors)
	{
		double distance = FVector::Dist(target->GetActorLocation(), GetActorLocation());

		if (nearestLength < distance)
		{
			continue;
		}

		nearestLength = distance;
		nearestWeapon = target;
	}

	return nearestWeapon;
}

void AShootingGameCodeCharacter::ReqReload_Implementation() // 서버에서 실행되고
{
	// 클라이언트로 전달
	ResReload();
}

void AShootingGameCodeCharacter::ResReload_Implementation()
{
	//InterfaceObj는 IWeaponInterface의 형변환
	//인터페이스가 있는 무기 레퍼런스인지 확인
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(EquipWeapon);
	
	if (InterfaceObj == nullptr)
	{
		return;
	}
	// if(!InterfaceObj)
	
	//유효하면 Reload실행
	InterfaceObj->Execute_EventReload(EquipWeapon);
}

void AShootingGameCodeCharacter::ReqShoot_Implementation()
{
	ResShoot();
}

void AShootingGameCodeCharacter::ResShoot_Implementation()
{
	IWeaponInterface* InterfaceObj = Cast<IWeaponInterface>(EquipWeapon);
	if(InterfaceObj == nullptr)
	{
		return;
	}

	InterfaceObj->Execute_EventTrigger(EquipWeapon);
}

FRotator AShootingGameCodeCharacter::GetPlayerRotation()
{
	ACharacter* pChar0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (pChar0 == this)
	{
		return GetControlRotation();
	}
	
	return PlayerRotation;
}

bool AShootingGameCodeCharacter::IsEquip()
{
	return IsValid(EquipWeapon);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShootingGameCodeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShootingGameCodeCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShootingGameCodeCharacter::Look);

		//Shoot
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AShootingGameCodeCharacter::Shoot);

		//Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AShootingGameCodeCharacter::Reload);

		//PressF
		EnhancedInputComponent->BindAction(PressFAction, ETriggerEvent::Started, this, &AShootingGameCodeCharacter::PressF);

		//Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &AShootingGameCodeCharacter::Drop);

	}

}

void AShootingGameCodeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AShootingGameCodeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShootingGameCodeCharacter::Shoot(const FInputActionValue& Value)
{
	ReqShoot();
}

void AShootingGameCodeCharacter::Reload(const FInputActionValue& Value)
{
	ReqReload();
}

void AShootingGameCodeCharacter::PressF(const FInputActionValue& Value)
{
	ReqPressF();
}

void AShootingGameCodeCharacter::Drop(const FInputActionValue& Value)
{
	ReqDrop();
}






