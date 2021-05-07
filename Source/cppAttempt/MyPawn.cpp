#include "MyPawn.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
#include "FindableActor.h"
#include <cppAttempt\PlayAgain.h>
#include "MyGameModeBase.h"



// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// setup vars.
	scene_root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	scene_root->CreationMethod = EComponentCreationMethod::Native;
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->CreationMethod = EComponentCreationMethod::Native;
	camera->AttachToComponent(scene_root, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyPawn::OnScreenTouch);

}

// What to do when the screen is touched (takes which finger was used to touch screen & where the 2d screen was touched)
void AMyPawn::OnScreenTouch(const ETouchIndex::Type fingerIndex, const FVector screenPosition)
{
	// Get the player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	// Get the touch state.
	playerController->APlayerController::GetInputTouchState(ETouchIndex::Touch1, /*out*/ touchX, /*out*/ touchY, /*out*/ touchTrue);
	// convert co-ords to vector
	FVector2D touchVector = UKismetMathLibrary::MakeVector2D(touchX, touchY);
	// Perform a hitTest, get the return values as hitTesTResult
	if (!WorldHitTest(touchVector, hitTestResult))
	{
		// HitTest returned false, get out.
		return;
	}
	
	// This is messy, pulls lots of info out of FHitResult structure, which we create in WorldHitTest()
	UGameplayStatics::BreakHitResult(
		hitTestResult, /*out*/ blockingHit, /*out*/ initialOverlap,	/*out*/ hitTime, /*out*/ hitDistance,
		/*out*/ hitLocation, /*out*/ hitImpactPoint, /*out*/ hitNormal, /*out*/ hitImpactNormal,
		/*out*/ hitMaterial, /*out*/ hitActor, /*out*/ hitComponent, /*out*/ hitBone,
		/*out*/ hitItem, /*out*/ hitFace, /*out*/ hitTraceStart, /*out*/ hitTraceEnd);
	// Get object of actor hit.
	UClass* hitActorObject = UGameplayStatics::GetObjectClass(hitActor);
	// get gamemode
	AMyGameModeBase* gm = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
	// if we've hit a target.
	if (UKismetMathLibrary::ClassIsChildOf(hitActorObject, AFindableActor::StaticClass()))
	{
		gm->TargetHit();
	} 
	else
	{
		// it's a game-over cube / play-again cube
		if (Cast<APlayAgain>(hitActor)->isPlayAgain()) {
			// Play again
			UKismetSystemLibrary::PrintString(this, FString(TEXT("play again")), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);

		} 
		else
		{
			UKismetSystemLibrary::PrintString(this, FString(TEXT("quit game")), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);

		}
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

/*
*	Take a screen hit, translate to 3d ray trace, return bool and hit result (detailing objects hit, if successful)
*/
bool AMyPawn::WorldHitTest(FVector2D screenPos, /*out*/ FHitResult& hitResult)
{
	bool returnValue = false;
	TArray<AActor*> temporaryActor{};

	// Create array of possible types that could be hit by trace vector
	TArray<TEnumAsByte<EObjectTypeQuery>> temporaryArray{};
	temporaryArray.SetNum(6, true);
	temporaryArray[0] = EObjectTypeQuery::ObjectTypeQuery1;
	temporaryArray[1] = EObjectTypeQuery::ObjectTypeQuery2;
	temporaryArray[2] = EObjectTypeQuery::ObjectTypeQuery3;
	temporaryArray[3] = EObjectTypeQuery::ObjectTypeQuery4;
	temporaryArray[4] = EObjectTypeQuery::ObjectTypeQuery5;
	temporaryArray[5] = EObjectTypeQuery::ObjectTypeQuery6;

	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection;
	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, /*out*/ worldPosition, /*out*/ worldDirection);

	// construct trace vector (from point clicked to *1000.0 beyond in same direction)
	FVector traceVector = UKismetMathLibrary::Multiply_VectorFloat(worldDirection, 1000.0);
	traceVector = UKismetMathLibrary::Add_Vector4Vector4(worldPosition, traceVector);

	// perform line trace
	bool traceSuccess = UKismetSystemLibrary::LineTraceSingleForObjects(
		this, worldPosition, traceVector, temporaryArray, false, temporaryActor,
		EDrawDebugTrace::None, /*out*/ hitResult, true,
		FLinearColor(1.000000, 0.000000, 0.000000, 1.000000), FLinearColor(0.000000, 1.000000, 0.000000, 1.000000), 5.000000);

	// return if the operation was successful
	return traceSuccess;
}

