// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootingPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHP_TwoParams, float, CurHP, float, MaxHP);

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

public:
	UFUNCTION(BlueprintCallable)
	void AddDamage(float Damage);

public:
	//���ø�����Ʈ -> OnRep
	UPROPERTY(ReplicatedUsing = OnRep_CurHP)
	float CurHP;

	//���ø�����Ʈ -> OnRep
	UPROPERTY(ReplicatedUsing = OnRep_MaxHP)
	float MaxHP;

	//�̺�Ʈ �����ķ� ����ϱ� ���� Ű����
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHP_TwoParams Fuc_Dele_UpdataeHP;
};
