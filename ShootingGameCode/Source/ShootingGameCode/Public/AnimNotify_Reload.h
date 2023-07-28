// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Reload.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API UAnimNotify_Reload : public UAnimNotify
{
	GENERATED_BODY()

public:
	//Notify 함수 정의
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
