// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Weapon.generated.h"

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
	void EventTrigger();

	//���� eventTrigger_implementation �Լ� �������̵�
	virtual void EventTrigger_Implementation() override;

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

	// BP�������̽� �⺻ ����

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// WeaponMesh��� ����ƽ �޽� ������Ʈ ����
	class UStaticMeshComponent* WeaponMesh;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//�ִ� ��Ÿ�� ���۷��� ShootMontage ���� ����
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//�ִ� ��Ÿ�� ���۷��� reloadMontage ���� ����
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//��ƼŬ �ý��� ���۷��� ShootEffect ���� ����
	UParticleSystem* ShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//���� ���̽� ���۷��� ShootSound ���� ����
	USoundBase* ShootSound;

	UPROPERTY(BlueprintReadWrite)
	//ĳ���� ������Ʈ ���۷��� OwnChar ���� ����
	ACharacter* OwnChar;
};
