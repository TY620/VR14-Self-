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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventTrigger();
	//EventTrigger �Լ� ����(implementation�� ����ؾ��ϴ�)

	virtual void EventTrigger_Implementation() override;
	//���� eventTrigger_implementation �Լ� �������̵�

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventReload();
	//EventReload �Լ� ����(implementation�� ����ؾ��ϴ�)

	virtual void EventReload_Implementation() override;
	//���� eventReload_implementation �Լ� �������̵�
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventShoot();
	//EventShoot �Լ� ����(implementation�� ����ؾ��ϴ�)

	virtual void EventShoot_Implementation() override;
	//���� eventShoot_implementation �Լ� �������̵�

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventPickUp(ACharacter* targetChar);

	virtual void EventPickUp_Implementation(ACharacter* targetChar) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventDrop(ACharacter* targetChar);

	virtual void EventDrop_Implementation(ACharacter* targetChar) override;

	// BP�������̽� �⺻ ����

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;
	// WeaponMesh��� ����ƽ �޽� ������Ʈ ����


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage;
	//�ִ� ��Ÿ�� ���۷��� ShootMontage ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage;
	//�ִ� ��Ÿ�� ���۷��� reloadMontage ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ShootEffect;
	//��ƼŬ �ý��� ���۷��� ShootEffect ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ShootSound;
	//���� ���̽� ���۷��� ShootSound ���� ����

	UPROPERTY(BlueprintReadWrite)
	ACharacter* OwnChar;
	//ĳ���� ������Ʈ ���۷��� OwnChar ���� ����
};
