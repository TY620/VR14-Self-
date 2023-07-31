// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h" // ����ü
#include "Weapon.generated.h"

class AWeapon;

// ����ü
USTRUCT(BlueprintType)
struct FST_Weapon : public FTableRowBase
{
	GENERATED_BODY()

public:
	//�⺻ ������
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
	//Weapon�� Actor, WeaponInterface�� ����� ����
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
	// �ؿ� �ִ� �Լ����� WeaponInterface���� ���Ǵ� �Լ��� �ݵ�� implementation �Լ��� �����ؾ� ��

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventTrigger �Լ� ����(Implementation�� ����ؾ��ϴ�)
	void EventTrigger(bool IsPress);

	//���� eventTrigger_implementation �Լ� �������̵�
	virtual void EventTrigger_Implementation(bool IsPress) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventReload �Լ� ����(implementation�� ����ؾ��ϴ�)
	void EventReload();

	//���� eventReload_implementation �Լ� �������̵�
	virtual void EventReload_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventShoot �Լ� ����(implementation�� ����ؾ��ϴ�)
	void EventShoot();

	//���� eventShoot_implementation �Լ� �������̵�
	virtual void EventShoot_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventPickUp �Լ� ����(implementation�� ����ؾ��ϴ�)
	void EventPickUp(ACharacter* targetChar);

	//���� eventpickup_implementation �Լ� �������̵�
	virtual void EventPickUp_Implementation(ACharacter* targetChar) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//EventDrop �Լ� ����(implementation�� ����ؾ��ϴ�)
	void EventDrop(ACharacter* targetChar);

	//���� eventdrop_implementation �Լ� �������̵�
	virtual void EventDrop_Implementation(ACharacter* targetChar) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//��ȿ�� �˻� �Լ� ����(implementation�� ����ؾ��ϴ�)
	void IsCanPickUp(bool& IsCanPickUp);

	//���� IsCanPickUp_implementation �Լ� �������̵�
	virtual void IsCanPickUp_Implementation(bool& IsCanPickUp) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventResetAmmo();

	virtual void EventResetAmmo_Implementation() override;

	// BP�������̽� �⺻ ����

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
	// WeaponMesh��� ����ƽ �޽� ������Ʈ ����
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
