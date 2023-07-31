// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h" // 구조체
#include "Weapon.generated.h"

class AWeapon;

// 구조체
USTRUCT(BlueprintType)
struct FST_Weapon : public FTableRowBase
{
	GENERATED_BODY()

public:
	//기본 생성자
	FST_Weapon() : StaticMesh(nullptr), ShootMontage(nullptr), ReloadMontage(nullptr), SoundBase(nullptr), FireEffect(nullptr), MaxAmmo(30), Damage(10), WeaponClass(nullptr)
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* SoundBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* FireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponClass;
};

UCLASS()
class SHOOTINGGAMECODE_API AWeapon : public AActor, public IWeaponInterface
	//Weapon은 Actor, WeaponInterface의 상속을 받음
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 밑에 있는 함수들은 WeaponInterface에서 사용되는 함수라서 반드시 implementation 함수로 정의해야 함

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventTrigger 함수 생성(Implementation을 사용해야하는)
	void EventTrigger(bool IsPress);

	//가상 eventTrigger_implementation 함수 오버라이딩
	virtual void EventTrigger_Implementation(bool IsPress) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventReload 함수 생성(implementation을 사용해야하는)
	void EventReload();

	//가상 eventReload_implementation 함수 오버라이딩
	virtual void EventReload_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventShoot 함수 생성(implementation을 사용해야하는)
	void EventShoot();

	//가상 eventShoot_implementation 함수 오버라이딩
	virtual void EventShoot_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventPickUp 함수 생성(implementation을 사용해야하는)
	void EventPickUp(ACharacter* targetChar);

	//가상 eventpickup_implementation 함수 오버라이딩
	virtual void EventPickUp_Implementation(ACharacter* targetChar) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventDrop 함수 생성(implementation을 사용해야하는)
	void EventDrop(ACharacter* targetChar);

	//가상 eventdrop_implementation 함수 오버라이딩
	virtual void EventDrop_Implementation(ACharacter* targetChar) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//유효성 검사 함수 생성(implementation을 사용해야하는)
	void IsCanPickUp(bool& IsCanPickUp);

	//가상 IsCanPickUp_implementation 함수 오버라이딩
	virtual void IsCanPickUp_Implementation(bool& IsCanPickUp) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventResetAmmo();

	virtual void EventResetAmmo_Implementation() override;

	// BP인터페이스 기본 구조

public:
	UFUNCTION(Server, Reliable)
	void ReqShoot(FVector vStart, FVector vEnd);
	
	UFUNCTION()
	void OnRep_Ammo();

	UFUNCTION()
	void OnRep_RowName();

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsCanShoot() const;

public:
	float GetFireStartLength();

	bool UseAmmo();

	void UpdateAmmoToHUD(int NewAmmo);

	void SetAmmo(int NewAmmo);

	void SetWeaponData(FName name);

	void SetWeaponRowName(FName name);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// WeaponMesh라는 스태틱 메쉬 컴포넌트 생성
	class UStaticMeshComponent* WeaponMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UAnimMontage* ShootMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UAnimMontage* ReloadMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UParticleSystem* ShootEffect;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USoundBase* ShootSound;

	UPROPERTY(BlueprintReadWrite)
	ACharacter* OwnChar;

	UPROPERTY(ReplicatedUsing = OnRep_Ammo)
	int Ammo;

	UPROPERTY(ReplicatedUsing = OnRep_RowName)
	FName RowName;
		
	FST_Weapon* weaponData;

};
