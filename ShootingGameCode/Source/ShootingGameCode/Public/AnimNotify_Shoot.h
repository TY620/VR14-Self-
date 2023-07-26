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
	//Notify 함수 정의
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// MeshComp: 애니메이션과 스켈레탈 메시에 접근
	// Animation: 발생한 애니메이션 노티피케이션과 연관된 애니메이션 시퀀스 또는 기반 클래스
	// EventReference : 노티피케이션 이벤트에 대한 참조를 나타내는 구조체. 어떤 종류의 노티피케이션이 발생했는지 등의 정보에 접근
	// 상위 클래스에 있는 Notify 함수를 파생 클래스에서 재정의하면, 
	// 해당 애니메이션 노티피케이션이 발생했을 때 파생 클래스에서 원하는 로직을 처리할 수 있음. 
	// 이를 통해 캐릭터 애니메이션의 특정 이벤트에 반응하여 원하는 행동을 수행하거나, 애니메이션 시퀀스에 관련된 다른 작업을 수행할 수 있음
	
};
