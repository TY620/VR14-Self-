// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h" // bulid.cs에 ItemPlugin 추가 필요
#include "GameFramework/Character.h"
#include "InputActionValue.h" 
#include "ShootingGameCodeCharacter.generated.h"


UCLASS(config = Game)
class AShootingGameCodeCharacter : public ACharacter, public IItemInterface
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

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* TriggerAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ReloadAction;

	/** PressF Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PressFAction;

	/** Drop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* DropAction;

	/** Grenade Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* GrenadeAction;

	//DefaultMappingContext에 IA jump, IA move, IA look, IA Trigger, IA reload, IA PressF, IA Drop..등 생성

public:
	AShootingGameCodeCharacter();


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void TriggerPress(const FInputActionValue& Value);
	void TriggerRelease(const FInputActionValue& Value);

	/** Called for looking input */
	void Reload(const FInputActionValue& Value);

	/** Called for looking input */
	void PressF(const FInputActionValue& Value);

	/** Called for looking input */
	void Drop(const FInputActionValue& Value);

	/** Called for looking input */
	void GrenadePress(const FInputActionValue& Value);
	void GrenadeRelease(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	//AActor 정의 안에 있는 TakeDamage 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnCharacterDestroyed(AActor* DestroyedActor);

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

	UFUNCTION(NetMulticast, Reliable)
		void ResPressF(AActor* weapon);

	UFUNCTION(Server, Reliable)
		void ReqDrop();

	UFUNCTION(NetMulticast, Reliable)
		void ResDrop();

	UFUNCTION(NetMulticast, Reliable)
		void ResRevive(FTransform ReviveTrans);

	UFUNCTION(Server, Reliable)
		void ReqGrenade();

	UFUNCTION(NetMulticast, Reliable)
		void ResGrenade();

	UFUNCTION(Server, Reliable)
		void ReqSpawnGrenade(FVector Start, FVector Impluse);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventGetItem(EItemType itemType);

	void EventGetItem_Implementation(EItemType itemType) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventUpdateNameTag();

	void EventUpdateNameTag_Implementation();
public:
	UFUNCTION(BlueprintCallable)
		//TSubclassOf : AWeapon을 상속 받은 클래스(본인포함)는 입력 값을 받을 수 있음
		void EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable)
		AActor* FindNearestWeapon();

	UFUNCTION(BlueprintCallable)
		void DoRagdoll();

	UFUNCTION(BlueprintCallable)
		void DoGetUp();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnUpdateHp(float CurHp, float MaxHp);

	void OnUpdateHp_Implementation(float CurHp, float MaxHp);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	void DoPickUp(AActor* weapon);

	void DoDrop();

	void BindPlayerState();

	void DoRevive();

	FTransform GetRandomReviveTransform();

	void CreateNameTag();

	void SpawnGrenade();

	void ShowGrenadeGuideLine();

public:
	UPROPERTY(Replicated)
		// 리플리케이트 설정
		FRotator PlayerRotation;

	UFUNCTION(BlueprintPure)
		//플레이어 0(index) 인지 확인하여 회전 값 전달 받는 함수
		FRotator GetPlayerRotation();

	AActor* EquipWeapon;

	UFUNCTION(BlueprintPure)
		bool IsEquip();

	bool IsRagdoll;

	FTimerHandle th_BindPlayerState;
	FTimerHandle th_Revive;
	FTimerHandle th_NameTag;
	FTimerHandle th_Grenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UCustomUserWidget> NameTagClass;

	UPROPERTY(BlueprintReadWrite)
		UCustomUserWidget* NameTagWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* GrenadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AGrenade> GrenadeClass;
};

