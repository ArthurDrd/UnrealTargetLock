// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Components/UTLHealthComponent.h"

#pragma region Base Functions

//////////////////////////////////////////////////////////////////////////
/// Base Functions
//////////////////////////////////////////////////////////////////////////

// Sets default values for this component's properties
UUTLHealthComponent::UUTLHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UUTLHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion