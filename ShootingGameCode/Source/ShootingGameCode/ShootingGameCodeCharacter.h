// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ShootingGameCodeCharacter.generated.h"


UCLASS(config=Game)
class AShootingGameCodeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	// InputMappingContext 레퍼런스의 DefaultMappingContext 변수 생성
	class UInputMappingContext* DefaultMappingContext;
	// DefaultMappingContext == IMC

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TriggerAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/** PressF Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PressFAction;

	/** Drop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropAction;

	//DefaultMappingContext에 IA jump, IA move, IA look, IA Trigger, IA reload, IA PressF, IA Drop칸을 생성

public:
	AShootingGameCodeCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Shoot input */
	void TriggerPress(const FInputActionValue& Value);
	void TriggerRelease(const FInputActionValue& Value);


	/** Called for Reload input */
	void Reload(const FInputActionValue& Value);

	/** Called for PressF input */
	void PressF(const FInputActionValue& Value);

	/** Called for Drop input */
	void Drop(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	//AActor 정의 안에 있는 TakeDamage 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


public:
	//소유 클라이언트에서 호출하고 서버에서 실행되는
	UFUNCTION(Server, Reliable)
	void ReqTrigger(bool IsPress);

	//서버에서 호출하고, 서버와 모든 클라이언트에서 실행되는
	UFUNCTION(NetMulticast, Reliable)
	void ResTrigger(bool IsPress);

	UFUNCTION(Server, Reliable)
	void ReqReload();

	UFUNCTION(NetMulticast, Reliable)
	void ResReload();

	UFUNCTION(Server, Reliable)
	void ReqPressF();

	UFUNCTION()
	void OnRep_EquipWeapon();

	UFUNCTION(Server, Reliable)
	void ReqDrop();

	UFUNCTION(NetMulticast, Reliable)
	void ResDrop();

public:
	//TSubclassOf : AWeapon을 상속 받은 클래스(본인포함)는 입력 값을 받을 수 있음
	UFUNCTION(BlueprintCallable)
	void EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable)
	AActor* FindNearestWeapon();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// 리플리케이트 설정
	// UPROPERTY(ReplicatedUsing = OnFuc) notify -> ???
	UPROPERTY(Replicated)

	//로테이터 변수 생성
	FRotator PlayerRotation;

	//플레이어 0(index) 인지 확인하여 회전 값 전달 받는 함수
	UFUNCTION(BlueprintPure)
	FRotator GetPlayerRotation();

	// 액터 클래스 EquipWeapon변수 생성
	// 리플리케이트 설정
	UPROPERTY(ReplicatedUsing = OnRep_EquipWeapon)
	AActor* EquipWeapon;

	UFUNCTION(BlueprintPure)
	bool IsEquip();
};

