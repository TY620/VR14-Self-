// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwitchTool.generated.h"

UCLASS()
class HELLOUNREALCODE_API ASwitchTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UFUNCTION()
	void OnSwitchBeginOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSwitchEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAcess = "true"))
		class UStaticMeshComponent* StaticMesh;

	//�۷ι� ���� �����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// TObjectPtr<AActor> Actor; // ���͸� ��ӹ��� �ֵ��� ���� ���� �� ����
	// TObjcetPtr : ������
	TArray<TObjectPtr<AActor>> Actors; // �迭��

};
