// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"

#include "Components/CapsuleComponent.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include <Runtime\Engine\Classes\GameFramework\HUD.h>

#include <Runtime\Engine\Classes\Kismet\KismetSystemLibrary.h>
#include <cppAttempt\MyGameModeBase.h>
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
#include "DrawDebugHelpers.h"


AMyHUD::AMyHUD()
{
	PrimaryActorTick.bCanEverTick = false;
	prompt_cleared = false;

	const ConstructorHelpers::FObjectFinder<UTexture2D> topInfoBarTextureAsset(TEXT("Texture2D'/Engine/EditorResources/TilePatine_N.TilePatine_N'"));
	if (topInfoBarTextureAsset.Object) topInfoBarTexture_ = topInfoBarTextureAsset.Object;
	
	const ConstructorHelpers::FObjectFinder<UTexture2D> buttonAsset(TEXT("Texture2D'/Game/HandheldARBP/Textures/rounded_button.rounded_button'"));
	if (buttonAsset.Object) buttonTexture_ = buttonAsset.Object;
	
	ConstructorHelpers::FObjectFinder<UFont> uiFontAsset{ TEXT("Font'/Engine/EngineFonts/DroidSansMono'") };

	if (uiFontAsset.Object) uiFont_ = uiFontAsset.Object;
	
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AMyHUD::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}

void AMyHUD::NotifyHitBoxClick(const FName BoxName)
{

	if (BoxName == FName(TEXT("hitbox1"))) {
		prompt_cleared = true;
		AMyGameModeBase* gm = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
		gm->StartAgain();
	}

	if (BoxName == FName(TEXT("hitbox2"))) {
		AMyGameModeBase* gm = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode());
		gm->QuitPlease();
	}

	Super::NotifyHitBoxClick(BoxName);
}

void AMyHUD::DrawHUD() 
{

	Super::DrawHUD();

	if (GEngine && GEngine->GameViewport)
	{
		FVector2D viewportSize;
		GEngine->GameViewport->GetViewportSize(viewportSize);
		uiScale_ = viewportSize.X / 2048.f;
	}
	else {
		UKismetSystemLibrary::PrintString(this, FString(TEXT("HUD ISSUE.")), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);

	}

	if (!prompt_cleared) {
		// show overlay.
		AMyHUD::ShowMenu();
	} 
	else
	{
		AMyHUD::ShowOverlay();
	}
}


// During gameplay show progress and time. Give cube cues on spawn in a neat diagram maybe?
void AMyHUD::ShowOverlay()
{
	// Get Screen dimensions.
	const float gWidth = GEngine->GameViewport->Viewport->GetSizeXY().X;
	const float gHeight = GEngine->GameViewport->Viewport->GetSizeXY().Y;

	// Draw prompt background
	FCanvasTileItem tileItem(FVector2D(0,gHeight*(9.0/10)), topInfoBarTexture_->Resource, FVector2D(gWidth * (1.0/6), gHeight/10), FLinearColor::White);
	Canvas->DrawItem(tileItem);

	// Get details on score, target and start time.
	int s = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode())->GetHUDScore();
	int t = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode())->GetHUDTarget();
	FDateTime start = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode())->GetHUDStart();

	DrawText(FString::Printf(TEXT("Progress: %d/%d"), s, t), FLinearColor::White, 10, gHeight * (29.0/30), uiFont_, 1);
	FTimespan elapsed = UKismetMathLibrary::Subtract_DateTimeDateTime(FDateTime::Now(), start);
	DrawText(FString::Printf(TEXT("Time: %s"), *elapsed.ToString()), FLinearColor::White, 10, gHeight * (27.5 / 30), uiFont_, 1);

	FRotator DesiredAngleZ = GetOwner()->GetActorRotation();
	FVector XVector = DesiredAngleZ.RotateVector(FVector(0, 1, 0));
	FVector YVector = DesiredAngleZ.RotateVector(FVector(0, 0, 1));

	DrawDebugLine(GetWorld(), FVector(500, 3, 3), FVector(500, -3, -3), FColor::Red, true, -1, 0, 1);
	DrawDebugLine(GetWorld(), FVector(500, 3, -3), FVector(500, -3, 3), FColor::Red, true, -1, 0, 1);


	// Draw the box.
		// bottom
	DrawLine(gWidth / 3, gHeight * (39.0 / 40), gWidth * (2.0 / 3), gHeight * (39.0 / 40), FLinearColor::White, 10);
		// left
	DrawLine(gWidth / 3, gHeight * (31.0 / 40), gWidth * (1.0 / 3), gHeight * (39.0 / 40), FLinearColor::White, 10);
		// right
	DrawLine(gWidth * (2.0 / 3), gHeight * (31.0 / 40), gWidth * (2.0 / 3), gHeight * (39.0 / 40), FLinearColor::White, 10);
		// top
	DrawLine(gWidth * (1.0 / 3), gHeight * (31.0 / 40), gWidth * (2.0 / 3), gHeight * (31.0 / 40), FLinearColor::White, 10);
	
	DrawLine(gWidth / 2 - 3, gHeight * (7.0 / 8) - 3, gWidth / 2 + 3, gHeight * (7.0 / 8) + 3, FLinearColor::Red, 2);
	DrawLine(gWidth / 2 - 3, gHeight * (7.0 / 8) + 3, gWidth / 2 + 3, gHeight * (7.0 / 8) - 3, FLinearColor::Red, 2);

	FVector box_loc = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode())->GetLastBox();
	float x_delta =	(500+box_loc.Y)/1000.0;	//(((box_loc.Y + 500) / 1000.0) * (gWidth / 3.0));
	float y_delta = (300-box_loc.Z) / 600;	//(((box_loc.Z - 300) / 600.0) * (gHeight * (8.0 / 40.0)));
	float base_x = gWidth / 3.0;
	float base_y = gHeight * (31.0 / 40.0);
	float x_width = base_x;
	float y_width = gHeight * (8.0 / 40.0);
	float box_x = base_x + (x_width * x_delta) - 10;
	float box_y = base_y + (y_width * y_delta) - 10;
	DrawRect(FLinearColor::Blue, box_x, box_y, 20, 20);

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f"), box_x), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f"), box_y), true, true, FLinearColor(0.000000, 0.660000, 1.000000, 1.000000), 2.000000);

}

void AMyHUD::ShowMenu()
{
	// Get Screen dimensions.
	const float gWidth = GEngine->GameViewport->Viewport->GetSizeXY().X;
	const float gHeight = GEngine->GameViewport->Viewport->GetSizeXY().Y;

	// Draw prompt background
	FCanvasTileItem tileItem(FVector2D::ZeroVector, topInfoBarTexture_->Resource, FVector2D(gWidth, gHeight), FLinearColor::White);
	Canvas->DrawItem(tileItem);
	
	// Text prompt
	DrawText(FString("RACE!\nHit 10 cubes as fast\nas you can"), FLinearColor::White, gWidth/3, gHeight/4, uiFont_, 2.5);

	// Start button with text description.
	FCanvasTileItem startButton(FVector2D(gWidth * 0.5, gHeight * 0.75), buttonTexture_->Resource, FVector2D(gWidth * 0.125, gHeight * 0.125), FLinearColor::Green);
	Canvas->DrawItem(startButton);
	DrawText(FString("Start"), FLinearColor::White, gWidth * 0.55, gHeight * 0.8, uiFont_, 1);

	// Start button hitbox
	AddHitBox(FVector2D(gWidth * 0.5, gHeight * 0.75), FVector2D(gWidth * 0.125, gHeight * 0.125), FName(TEXT("hitbox1")), true);

	// Get best time, display if one exists.
	FTimespan best = Cast<AMyGameModeBase>(GetWorld()->GetAuthGameMode())->GetBestTime();
	if (best.GetTicks() != FTimespan::MaxValue().GetTicks()) {
		DrawText(FString::Printf(TEXT("high score: %s"), *best.ToString()), FLinearColor::Black, gWidth * 0.33, gHeight * 0.9, uiFont_, 1, true);
	} 
	else
	{
		DrawText(FString::Printf(TEXT("no high score yet")), FLinearColor::Black, gWidth * 0.33, gHeight * 0.9, uiFont_, 1, true);
	}

	// Quit button with text description.
	FCanvasTileItem menuButton(FVector2D(gWidth * 0.25, gHeight * 0.75), buttonTexture_->Resource, FVector2D(gWidth * 0.125, gHeight * 0.125), FLinearColor::Red);
	Canvas->DrawItem(menuButton);
	DrawText(FString("Menu"), FLinearColor::White, gWidth * 0.3, gHeight * 0.8, uiFont_, 1);

	// Quit button hitbox
	AddHitBox(FVector2D(gWidth * 0.25, gHeight * 0.75), FVector2D(gWidth * 0.125, gHeight * 0.125), FName(TEXT("hitbox2")), true);

}

void AMyHUD::SetPromptCleared(bool newValue)
{
	prompt_cleared = newValue;
}