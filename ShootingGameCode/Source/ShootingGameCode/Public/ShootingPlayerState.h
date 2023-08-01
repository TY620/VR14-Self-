// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootingPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP_TwoParams, float, CurHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_UpdateMag_OneParam, int, Mag);


/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API AShootingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AShootingPlayerState();

public:
	UFUNCTION()
	void OnRep_CurHP();

	UFUNCTION()
	void OnRep_MaxHP();

	UFUNCTION()
	void OnRep_Mag();

public:
	UFUNCTION(BlueprintCallable)
	void AddDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void AddMag();

	UFUNCTION(BlueprintCallable)
	bool UseMag();

	UFUNCTION(BlueprintPure)
	bool IsCanUseMag();

public:
	//���ø�����Ʈ -> OnRep
	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float CurHP;

	//���ø�����Ʈ -> OnRep
	UPROPERTY(ReplicatedUsing = OnRep_MaxHP)
	float MaxHP;

	UPROPERTY(ReplicatedUsing = OnRep_Mag)
	int Mag;


	//�̺�Ʈ �����ķ� ����ϱ� ���� Ű����
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHP_TwoParams Fuc_Dele_UpdataeHP;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateMag_OneParam Fuc_Dele_UpdateMag;
};
