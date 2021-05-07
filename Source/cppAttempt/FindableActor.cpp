// Fill out your copyright notice in the Description page of Project Settings.

#include "FindableActor.h"

#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"



// Sets default values
AFindableActor::AFindableActor()
{
 	
	sComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	sMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	sMesh->AttachToComponent(sComp, FAttachmentTransformRules::KeepRelativeTransform);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/HandheldARBP/Materials/Cube.Cube'"));
	UStaticMesh* Asset = MeshAsset.Object;
	sMesh->SetStaticMesh(Asset);

	UMaterial* ExampleMaterial = nullptr;
	ConstructorHelpers::FObjectFinder<UMaterial>MaterialAsset(TEXT("Material'/Game/HandheldARBP/Materials/basicBlue.basicBlue'"));
	ExampleMaterial = MaterialAsset.Object;
	sMesh->SetMaterial(0, ExampleMaterial);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFindableActor::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AFindableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

