// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveRect.h"

// Sets default values
AMoveRect::AMoveRect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(Root);
	StaticMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->SetRelativeTransform(FTransform::Identity);
}

// Called when the game starts or when spawned
void AMoveRect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoveRect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (MoveType)
	{
	case EN_MoveType::MoveRight:
	{
		StaticMesh->AddRelativeLocation(FVector(1, 0, 0));
		if (StaticMesh->GetRelativeLocation().X > 100)
		{
			MoveType = EN_MoveType :: MoveUp;
		}
	}
		break;
	case EN_MoveType::MoveUp:
	{
		StaticMesh->AddRelativeLocation(FVector(0, 0, 1));
		if (StaticMesh->GetRelativeLocation().Z > 100)
		{
			MoveType = EN_MoveType::MoveLeft;
		}

	}
		break;
	case EN_MoveType::MoveLeft:	
	{
		StaticMesh->AddRelativeLocation(FVector(-1, 0, 0));
		if (StaticMesh->GetRelativeLocation().X < 0)
		{
			MoveType = EN_MoveType::MoveDown;
		}
	}
		break;
	case EN_MoveType::MoveDown:
	{
		StaticMesh->AddRelativeLocation(FVector(0, 0, -1));
		if (StaticMesh->GetRelativeLocation().Z < 0)
		{
			MoveType = EN_MoveType::MoveRight;
		}
	}
		break;

	}
	

}

