// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveLeftRight.h"

// Sets default values
AMoveLeftRight::AMoveLeftRight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> sm(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube'"));
	//// ConstructorHelpers�� ������Ʈ�� ã�� ���� : StaticMesh�� ã��
	//// �𸮾󿡼� ����ƽ �Ž� ���۷��� �����ؼ� TEXT�� ����
	//
	//if (sm.Succeeded)
	//{
	//	StaticMesh->SetStaticMesh(sm.Object);
	//}


	// RootComponent = Root; 
	SetRootComponent(Root); 
	//��Ʈ�� ��Ʈ������Ʈ�� Set
	StaticMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	//��Ʈ ������Ʈ�� �θ�, ��ġ�� RelativeTransform
	StaticMesh->SetRelativeTransform(FTransform::Identity);
	// Identity : �⺻ ������ ���� - ��ġ 0,0,0 ������ 1,1,1
	IsMoveRight = true;
	IsPlay = false;

}

// Called when the game starts or when spawned
void AMoveLeftRight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoveLeftRight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsPlay)
	{
		if (IsMoveRight)
		{
			StaticMesh->AddRelativeLocation(FVector(1, 0, 0));
			//MoveRight�� x�������� 1����
			if (StaticMesh->GetRelativeLocation().X > 100)
			{
				IsMoveRight = false;
				//��ġ�� 0���� ũ�� ��������
			}
		}
		else
		{
			StaticMesh->AddRelativeLocation(FVector(-1, 0, 0));
			//MoveRight�� x�������� -1����
			if (StaticMesh->GetRelativeLocation().X < 0)
			{
				IsMoveRight = true;
				//��ġ�� 0���� ������ ������
			}

		}
	}
}

void AMoveLeftRight::EventOverlap_Implementaion(bool IsOverlap)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("EventOverlap IsOverlap=%s")
		, IsOverlap ? TEXT("True") : TEXT("False")));

	IsPlay = IsOverlap;

}

