// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTemplate/UnrealTargetLockGameMode.h"
#include "FPSTemplate/UnrealTargetLockCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealTargetLockGameMode::AUnrealTargetLockGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
