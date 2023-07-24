// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/SwitchTool.h"
#include "Tool/EventInterface.h"

// Sets default values
ASwitchTool::ASwitchTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(Root);
	StaticMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->SetRelativeTransform(FTransform::Identity);
	// = StaticMesh->SetupAttachment(Root);
	

	StaticMesh->SetCollisionProfileName("OverlapAllDynamic");
	// ����ƽ �Ž��� overlapalldynamic���� �ݸ��� ������ ������
	
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ASwitchTool::OnSwitchBeginOverlap);
	StaticMesh->OnComponentEndOverlap.AddDynamic(this, &ASwitchTool::OnSwitchEndOverlap);



	// (����� �Լ�, �Լ� ���� �Ŀ� OnComponnentBegin,EndOverlap�� �Ű������� ã�ƾ���. ���Ƿ� �̵�->���Ƿ� �̵� ��
	// �̸� ���� �ִ� �Ű������� �����Ͽ� �Լ� ���� �ʿ�)
}

// Called when the game starts or when spawned
void ASwitchTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitchTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitchTool::OnSwitchBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("BeginOverlap"));

	for (AActor* a : Actors)
	{
		IEventInterface* InterfaceObj = Cast<IEventInterface>(a);
		if (InterfaceObj)
		{
			InterfaceObj->Execute_EventOverlap(a, true);
		}
	}

}

void ASwitchTool::OnSwitchEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("EndOverlap"));

	for (AActor* a : Actors)
	{
		IEventInterface* InterfaceObj = Cast<IEventInterface>(a);
		if (InterfaceObj)
		{
			InterfaceObj->Execute_EventOverlap(a, false);
		}
	}


}



