// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API UAnimNotify_Shoot : public UAnimNotify
{
	GENERATED_BODY()

public:
	//Notify �Լ� ����
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// MeshComp: �ִϸ��̼ǰ� ���̷�Ż �޽ÿ� ����
	// Animation: �߻��� �ִϸ��̼� ��Ƽ�����̼ǰ� ������ �ִϸ��̼� ������ �Ǵ� ��� Ŭ����
	// EventReference : ��Ƽ�����̼� �̺�Ʈ�� ���� ������ ��Ÿ���� ����ü. � ������ ��Ƽ�����̼��� �߻��ߴ��� ���� ������ ����
	// ���� Ŭ������ �ִ� Notify �Լ��� �Ļ� Ŭ�������� �������ϸ�, 
	// �ش� �ִϸ��̼� ��Ƽ�����̼��� �߻����� �� �Ļ� Ŭ�������� ���ϴ� ������ ó���� �� ����. 
	// �̸� ���� ĳ���� �ִϸ��̼��� Ư�� �̺�Ʈ�� �����Ͽ� ���ϴ� �ൿ�� �����ϰų�, �ִϸ��̼� �������� ���õ� �ٸ� �۾��� ������ �� ����
	
};
