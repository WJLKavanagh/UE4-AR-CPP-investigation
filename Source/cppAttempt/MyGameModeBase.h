#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/DateTime.h"
class USceneComponent;
class AActor;
class AMyPawn;
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CPPATTEMPT_API AMyGameModeBase : public AGameModeBase
{
public:
	GENERATED_BODY()
	USceneComponent* defaultSceneRoot;
	AMyGameModeBase();
	UPROPERTY(EditAnywhere, meta=(Category="game"))
		int32 score;
		int32 target;
		float time_per_level;
		bool level_ongoing;
		int32 current_level;
		float max_spawn_value;
		float time_left;
		AActor* target_reference;
		AActor* play_again_actor;
		AActor* quit_actor;
		FTimerHandle level_timer_handle;
		FDateTime game_start;
		FDateTime game_end;
		APlayerController* plrCont;
		FTimespan best_time;
		FVector last_box;
	virtual void CoreLoop();
	virtual void WaitForLevel();
	virtual void StartLevel(int32 level, AActor*& targetActor);
	virtual void EndLevel(AActor* targetActor);
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void TargetHit();
	virtual void StartAgain();
	virtual void QuitPlease();
	virtual void NewGamePrompt();
	virtual FTimespan GetBestTime();
	int32 GetHUDScore();
	int32 GetHUDTarget();
	FVector GetLastBox();
	FDateTime GetHUDStart();
};
