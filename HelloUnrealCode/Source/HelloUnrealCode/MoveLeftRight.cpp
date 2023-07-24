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
	//// ConstructorHelpers의 오브젝트를 찾는 도구 : StaticMesh를 찾음
	//// 언리얼에서 스태틱 매쉬 레퍼런스 복사해서 TEXT에 넣음
	//
	//if (sm.Succeeded)
	//{
	//	StaticMesh->SetStaticMesh(sm.Object);
	//}


	// RootComponent = Root; 
	SetRootComponent(Root); 
	//루트를 루트컴포넌트로 Set
	StaticMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	//루트 컴포넌트가 부모, 위치는 RelativeTransform
	StaticMesh->SetRelativeTransform(FTransform::Identity);
	// Identity : 기본 값으로 세팅 - 위치 0,0,0 스케일 1,1,1
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
			//MoveRight면 x방향으로 1증가
			if (StaticMesh->GetRelativeLocation().X > 100)
			{
				IsMoveRight = false;
				//위치가 0보다 크면 거짓으로
			}
		}
		else
		{
			StaticMesh->AddRelativeLocation(FVector(-1, 0, 0));
			//MoveRight면 x방향으로 -1증가
			if (StaticMesh->GetRelativeLocation().X < 0)
			{
				IsMoveRight = true;
				//위치가 0보다 작으면 참으로
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

