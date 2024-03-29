// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UTLGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTARGETLOCK_API AUTLGameHUD : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Target Lock")
	void SetCrosshairPosition(const FVector2D& ScreenPosition);
};
