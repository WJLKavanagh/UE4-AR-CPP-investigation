#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCameraComponent;
class USceneComponent;
class MyGameModeBase;
class AActor;
class UPhysicalMaterial;
class UPrimitiveComponent;

UCLASS()
class CPPATTEMPT_API AMyPawn : public APawn
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, NonTransactional, meta = (Category = "Default", OverrideNativeName = "Camera"))
		UCameraComponent* camera;

	UPROPERTY(EditAnywhere, NonTransactional, meta = (Category = "Default", OverrideNativeName = "DefaultSceneRoot"))
		USceneComponent* scene_root;

	virtual void OnScreenTouch(ETouchIndex::Type fingerIndex, FVector screenPosition);

	float touchX;
	float touchY;
	bool touchTrue;
	FHitResult hitTestResult;
	bool blockingHit;
	bool initialOverlap;
	float hitTime;
	float hitDistance;
	FVector hitLocation;
	FVector hitImpactPoint;
	FVector hitNormal;
	FVector hitImpactNormal;
	UPhysicalMaterial* hitMaterial;
	AActor* hitActor;
	UPrimitiveComponent* hitComponent;
	FName hitBone;
	int32 hitItem;
	int32 hitFace;
	FVector hitTraceStart;
	FVector hitTraceEnd;

	// TODO: init gamemode.

	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool WorldHitTest(FVector2D screenPos, /*out*/ FHitResult& hitResult);

};
