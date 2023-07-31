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
	// InputMappingContext ���۷����� DefaultMappingContext ���� ����
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

	//DefaultMappingContext�� IA jump, IA move, IA look, IA Trigger, IA reload, IA PressF, IA Dropĭ�� ����

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

	//AActor ���� �ȿ� �ִ� TakeDamage �������̵�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


public:
	//���� Ŭ���̾�Ʈ���� ȣ���ϰ� �������� ����Ǵ�
	UFUNCTION(Server, Reliable)
	void ReqTrigger(bool IsPress);

	//�������� ȣ���ϰ�, ������ ��� Ŭ���̾�Ʈ���� ����Ǵ�
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
	//TSubclassOf : AWeapon�� ��� ���� Ŭ����(��������)�� �Է� ���� ���� �� ����
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
	// ���ø�����Ʈ ����
	// UPROPERTY(ReplicatedUsing = OnFuc) notify -> ???
	UPROPERTY(Replicated)

	//�������� ���� ����
	FRotator PlayerRotation;

	//�÷��̾� 0(index) ���� Ȯ���Ͽ� ȸ�� �� ���� �޴� �Լ�
	UFUNCTION(BlueprintPure)
	FRotator GetPlayerRotation();

	// ���� Ŭ���� EquipWeapon���� ����
	// ���ø�����Ʈ ����
	UPROPERTY(ReplicatedUsing = OnRep_EquipWeapon)
	AActor* EquipWeapon;

	UFUNCTION(BlueprintPure)
	bool IsEquip();
};

