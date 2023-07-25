// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Weapon.generated.h"

UCLASS()
class SHOOTINGGAMECODE_API AWeapon : public AActor, public IWeaponInterface
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

	virtual void EventTrigger_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventReload();

	virtual void EventReload_Implementation() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventShoot();

	virtual void EventShoot_Implementation() override;

	//BP�������̽� �⺻ ����

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* WeaponMesh;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ShootEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ShootSound;

	UPROPERTY(BlueprintReadWrite)
	ACharacter* OwnChar;
};
