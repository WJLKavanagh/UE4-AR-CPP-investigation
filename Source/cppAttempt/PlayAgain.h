// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayAgain.generated.h"

UCLASS()
class CPPATTEMPT_API APlayAgain : public AActor
{
	GENERATED_BODY()
	UPROPERTY(Category = "MyCategory", EditAnywhere)
		UStaticMeshComponent* sMesh;
	UPROPERTY(Category = "MyCategory", EditAnywhere)
		USceneComponent* sComp;
	UPROPERTY(Category = "MyCategory", VisibleAnywhere)
		USceneComponent* bKeepGoing;
	UPROPERTY(Category = "MyCategory", VisibleAnywhere)
		UMaterialInstanceDynamic* material;
	UPROPERTY(Category = "MyCategory", VisibleAnywhere)
		UMaterial* ExampleMaterial;
	UPROPERTY(Category = "MyCategory", VisibleAnywhere)
		bool play_again;


public:	
	// Sets default values for this actor's properties
	APlayAgain();
	virtual void SetUp(bool keepGoing);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool isPlayAgain();

};
