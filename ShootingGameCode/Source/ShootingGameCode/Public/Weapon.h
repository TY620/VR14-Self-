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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventTrigger();
	//EventTrigger 함수 생성(implementation을 사용해야하는)

	virtual void EventTrigger_Implementation() override;
	//가상 eventTrigger_implementation 함수 오버라이딩

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventReload();
	//EventReload 함수 생성(implementation을 사용해야하는)

	virtual void EventReload_Implementation() override;
	//가상 eventReload_implementation 함수 오버라이딩
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventShoot();
	//EventShoot 함수 생성(implementation을 사용해야하는)

	virtual void EventShoot_Implementation() override;
	//가상 eventShoot_implementation 함수 오버라이딩

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventPickUp(ACharacter* targetChar);

	virtual void EventPickUp_Implementation(ACharacter* targetChar) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventDrop(ACharacter* targetChar);

	virtual void EventDrop_Implementation(ACharacter* targetChar) override;

	// BP인터페이스 기본 구조

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;
	// WeaponMesh라는 스태틱 메쉬 컴포넌트 생성


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage;
	//애님 몽타주 레퍼런스 ShootMontage 변수 생성

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage;
	//애님 몽타주 레퍼런스 reloadMontage 변수 생성

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ShootEffect;
	//파티클 시스템 레퍼런스 ShootEffect 변수 생성

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ShootSound;
	//사운드 베이스 레퍼런스 ShootSound 변수 생성

	UPROPERTY(BlueprintReadWrite)
	ACharacter* OwnChar;
	//캐릭터 오브젝트 레퍼런스 OwnChar 변수 생성
};
