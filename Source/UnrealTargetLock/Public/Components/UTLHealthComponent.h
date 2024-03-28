// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTLHealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALTARGETLOCK_API UUTLHealthComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Base Functions

//////////////////////////////////////////////////////////////////////////
/// Base Functions
//////////////////////////////////////////////////////////////////////////
	
public:
	// Sets default values for this component's properties
	UUTLHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma endregion 
};
