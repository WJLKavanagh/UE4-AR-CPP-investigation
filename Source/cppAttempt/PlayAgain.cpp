// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayAgain.h"

// Sets default values
APlayAgain::APlayAgain()
{
	
	PrimaryActorTick.bCanEverTick = true;

	sComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	sMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	sMesh->AttachToComponent(sComp, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/HandheldARBP/Materials/Cube.Cube'"));
	UStaticMesh* Asset = MeshAsset.Object;
	sMesh->SetStaticMesh(Asset);

	ExampleMaterial = nullptr;
	ConstructorHelpers::FObjectFinder<UMaterial>MaterialAsset(TEXT("Material'/Game/HandheldARBP/Materials/basicBlue.basicBlue'"));
	ExampleMaterial = MaterialAsset.Object;
	play_again = false;

}

void APlayAgain::SetUp(bool keepGoing)
{

	play_again = keepGoing;
	
	if (keepGoing)
	{
		material->SetVectorParameterValue("Colour", FLinearColor::White);
	} 
	else
	{
		material->SetVectorParameterValue("Colour", FLinearColor::Red);
	}

}

// Called when the game starts or when spawned
void APlayAgain::BeginPlay()
{
	Super::BeginPlay();


	material = UMaterialInstanceDynamic::Create(ExampleMaterial, NULL);
	sMesh->SetMaterial(0, material);
	
}

// Called every frame
void APlayAgain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APlayAgain::isPlayAgain()
{
	return APlayAgain::play_again;
}


