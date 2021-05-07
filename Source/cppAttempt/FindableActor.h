// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
class UStaticMeshComponent;
class USceneComponent;
class UMaterialInstanceDynamic;
#include "FindableActor.generated.h"

UCLASS()
class CPPATTEMPT_API AFindableActor : public AActor
{
	GENERATED_BODY()
	UPROPERTY(Category = "MyCategory", EditAnywhere)
		UStaticMeshComponent* sMesh;
	UPROPERTY(Category = "MyCategory", EditAnywhere)
		USceneComponent* sComp;
	UPROPERTY(Category = "MyCategory", EditAnywhere)
		UMaterialInterface* dynamicMat;
		UMaterialInstanceDynamic* d_dynamicMat;
	/* UPROPERTY(Category = "MyCategory", EditAnywhere, meta = (DisplayName = "My Custom Mesh", AllowedClasses = "StaticMesh"))
		FSoftObjectPath CustomMesh;*/
	
public:	
	// Sets default values for this actor's properties
	AFindableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
