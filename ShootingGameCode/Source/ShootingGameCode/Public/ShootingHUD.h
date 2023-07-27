// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootingHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAMECODE_API AShootingHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* HUDWidget;
	
};
