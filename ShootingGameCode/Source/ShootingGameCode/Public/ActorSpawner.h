// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorSpawner.generated.h"

UCLASS()
class SHOOTINGGAMECODE_API AActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//Actor를 상속 받아 입력값을 만들 수 있는(변수 눈깔 열기?)
	// 블루프린트에서 SpawnClass에 Weapon을 넣어줌
	TSubclassOf<AActor> SpawnClass;

};
