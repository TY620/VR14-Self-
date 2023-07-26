// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Weapon.generated.h"

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
	void EventTrigger();

	//가상 eventTrigger_implementation 함수 오버라이딩
	virtual void EventTrigger_Implementation() override;

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

	// BP인터페이스 기본 구조

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// WeaponMesh라는 스태틱 메쉬 컴포넌트 생성
	class UStaticMeshComponent* WeaponMesh;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//애님 몽타주 레퍼런스 ShootMontage 변수 생성
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//애님 몽타주 레퍼런스 reloadMontage 변수 생성
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//파티클 시스템 레퍼런스 ShootEffect 변수 생성
	UParticleSystem* ShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//사운드 베이스 레퍼런스 ShootSound 변수 생성
	USoundBase* ShootSound;

	UPROPERTY(BlueprintReadWrite)
	//캐릭터 오브젝트 레퍼런스 OwnChar 변수 생성
	ACharacter* OwnChar;
};
