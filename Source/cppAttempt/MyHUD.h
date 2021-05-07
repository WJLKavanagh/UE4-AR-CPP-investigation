#pragma once


#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 *
 */
UCLASS()
class CPPATTEMPT_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMyHUD();
	virtual void DrawHUD() override;
	void ShowOverlay();
	void ShowMenu();
	void BeginPlay() override;
	void Tick(float deltaSeconds) override;
	void NotifyHitBoxClick(const FName BoxName) override;
	void SetPromptCleared(bool newValue);

private:
	// Main drawing function
	
	bool prompt_cleared;
	float uiScale_;
	UTexture2D* topInfoBarTexture_;
	UTexture2D* buttonTexture_;
	UFont* uiFont_;
};