// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingHUD.h"
//���� ���� ���� ���� include
#include "Blueprint/UserWidget.h"

void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}
