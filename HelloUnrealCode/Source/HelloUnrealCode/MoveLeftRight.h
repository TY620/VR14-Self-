// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Tool/EventInterface.h"
#include "GameFramework/Actor.h"
#include "MoveLeftRight.generated.h"

UCLASS()
class HELLOUNREALCODE_API AMoveLeftRight : public AActor, public IEventInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveLeftRight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EventOverlap(bool IsOverlap); // eventinterface¿« ≤Æµ•±‚?

	virtual void EventOverlap_Implementaion(bool IsOverlap) override;

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAcess = "true"))
	class UStaticMeshComponent* StaticMesh;

	bool IsMoveRight;
	bool IsPlay;

};
