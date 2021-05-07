#include "MyGameModeBase.h"
#include "cppAttempt.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/GameFramework/GameModeBase.h"
#include "Runtime/Engine/Classes/Kismet/KismetTextLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <string>
#include "FindableActor.h"
#include "PlayAgain.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"
//#include "GameFramework/HUD.h"
#include "MyHUD.h"
#include "MyPawn.h"
#include <Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

using namespace std;

AMyGameModeBase::AMyGameModeBase()
{
	defaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	defaultSceneRoot->CreationMethod = EComponentCreationMethod::Native;

	HUDClass = AMyHUD::StaticClass();
	//Cast<AMyHUD>(HUDClass)->DrawHUD();

	score = 0;
	target = 10;
	time_per_level = 4.0f;
	level_ongoing = false;
	current_level = 1;
	max_spawn_value = 300.0f;
	// time_left = 0.0f;						// TODO: WILL BE USED FOR UI TIMER
	DefaultPawnClass = AMyPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	game_start = FDateTime::Now();
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("The time is now: %s"), *game_start.ToString()), true, true, FLinearColor::Yellow, 5.0);
	best_time = UKismetMathLibrary::TimespanMaxValue();
	
}

// Start the game, keep the game running until score == target
void AMyGameModeBase::CoreLoop()
{
	if (score >= target)
	{
		
		game_end = FDateTime::Now();
		FTimespan time_taken = UKismetMathLibrary::Subtract_DateTimeDateTime(game_end, game_start);
		FString time_as_str{};
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("You took: %s"), *time_taken.ToString()), true, true, FLinearColor::Yellow, 5.000000);
		if (UKismetMathLibrary::LessEqual_TimespanTimespan(time_taken, best_time))
		{
			best_time = time_taken;
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("New Highscore")), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 5.0);
		}
		else 
		{
			FString best_time_as_str{};
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Highscore: %s"), *best_time.ToString()), true, true, FLinearColor::Yellow, 5.000000);
		}
		NewGamePrompt();
		return;
	}

	StartLevel(current_level, target_reference);
	//UKismetSystemLibrary::PrintText(this, UKismetTextLibrary::Conv_StringToText("waiting"), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);
	GetWorld()->GetTimerManager().SetTimer(level_timer_handle, this, &AMyGameModeBase::WaitForLevel, 0.1f, false, time_per_level);
	return;

}

// Called if timer for level expires. player didn't click cube in time, start next level.
void AMyGameModeBase::WaitForLevel()
{

	UKismetSystemLibrary::PrintText(this, UKismetTextLibrary::Conv_StringToText("Too slow!"), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);
	GetWorldTimerManager().ClearTimer(level_timer_handle);
	EndLevel(target_reference);

}

// Spawn a target cube in a random position
void AMyGameModeBase::StartLevel(int32 l, /*out*/ AActor*& targetActor)
{

	// generate values for random position
	float x = UKismetMathLibrary::RandomFloatInRange(100, 1000);			// back-forth ?
	float y = UKismetMathLibrary::RandomFloatInRange(-500, 500);			// left-right
	float z = UKismetMathLibrary::RandomFloatInRange(-300, 300);			// up-down

	// debug
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Coords: %f, %f, %f"), x, y, z), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), time_per_level);

	// turn vectors into transform for object spawn.
	FVector v = UKismetMathLibrary::MakeVector(x, y, z);
	last_box = v;
	FTransform fT = UKismetMathLibrary::Conv_VectorToTransform(v);

	// spawn object
	AActor* targetRef = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AFindableActor::StaticClass(), fT, ESpawnActorCollisionHandlingMethod::Undefined, ((AActor*)nullptr));
	targetActor = UGameplayStatics::FinishSpawningActor(targetRef, fT);

	// Get camera location and rotation
	FVector camLoc;
	FRotator camRot;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint( camLoc, camRot);


	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Coords: %f, %f, %f"), camLoc.X, camLoc.Y, camLoc.Z), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), time_per_level);
	FVector closestPoint = UKismetMathLibrary::FindClosestPointOnLine(v, camLoc, camRot.Vector());
	FVector difference = UKismetMathLibrary::Subtract_VectorVector(v, closestPoint);

	// DEBUG: Print handy direction in lieu of minimap.
	/*if (difference.Y > 0) {
		UKismetSystemLibrary::PrintString(this, "Right", true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), time_per_level);
	}
	else {
		UKismetSystemLibrary::PrintString(this, "Left", true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), time_per_level);
	}
	if (difference.Z > 0) {
		UKismetSystemLibrary::PrintString(this, "Up", true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), time_per_level);
	}
	else {
		UKismetSystemLibrary::PrintString(this, "Down", true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), time_per_level);
	}*/

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Coords: %f, %f, %f"), difference.X, difference.Y, difference.Z), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), time_per_level);
}

// end the level and destroy the target
void AMyGameModeBase::EndLevel(/*out*/ AActor* targetActor)
{
	
	if (::IsValid(targetActor))
	{
		targetActor->K2_DestroyActor(); 
	}
	level_ongoing = false;
	current_level++;
	//UKismetSystemLibrary::PrintText(this, UKismetTextLibrary::Conv_IntToText(current_level), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);
	AMyGameModeBase::CoreLoop();
}

// on tick: do nothing.
void AMyGameModeBase::Tick(float DeltaTime) 
{ 
	Super::Tick(DeltaTime);
}

// on start: do nothing
void AMyGameModeBase::BeginPlay()
{
	// TODO: Draw a cross at the origin?
}

// increment score, end level.
void AMyGameModeBase::TargetHit()
{
	score++;
	EndLevel(target_reference);
}

// Reset score and game timer, start game again.
void AMyGameModeBase::StartAgain()
{
	score = 0;
	game_start = FDateTime::Now();
	AMyGameModeBase::CoreLoop();
}

// Return to main menu
void AMyGameModeBase::QuitPlease()
{
	// TODO: load menu.
	UKismetSystemLibrary::PrintString(this, "Quit to menu", true, true, FLinearColor::Blue, 5.000000);

}

void AMyGameModeBase::NewGamePrompt()
{
	Cast<AMyHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD())->SetPromptCleared(false);
}

FTimespan AMyGameModeBase::GetBestTime()
{
	return best_time;
}

FDateTime AMyGameModeBase::GetHUDStart()
{
	return game_start;
}

int32 AMyGameModeBase::GetHUDScore()
{
	return score;
}

int32 AMyGameModeBase::GetHUDTarget()
{
	return target;
}

FVector AMyGameModeBase::GetLastBox()
{
	return last_box;
}