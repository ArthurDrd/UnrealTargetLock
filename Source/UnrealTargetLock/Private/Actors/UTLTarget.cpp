// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Actors/UTLTarget.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

#pragma region Base Functions

//////////////////////////////////////////////////////////////////////////
/// Base Functions
//////////////////////////////////////////////////////////////////////////

// Sets default values
AUTLTarget::AUTLTarget()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	TargetCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetCapsule"));
	TargetCapsule->SetupAttachment(RootComponent);

	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->SetupAttachment(TargetCapsule);
}

// Called when the game starts or when spawned
void AUTLTarget::BeginPlay()
{
	Super::BeginPlay();

	checkf(MaterialsList.Num() > 0, TEXT("MaterialsList is empty"));
}

void AUTLTarget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, TargetState);
}

#pragma endregion

#pragma region Target Lock

//////////////////////////////////////////////////////////////////////////
/// Target Lock
//////////////////////////////////////////////////////////////////////////

void AUTLTarget::SetState(const ETargetState NewState)
{
	if(!HasAuthority()) return; // Server authoritative
	
	if(NewState == TargetState) return; // Ensure stage is changing
	
	TargetState = NewState;
	OnRep_StateChanged(); // Call OnRep manually for the server
}

void AUTLTarget::OnRep_StateChanged()
{
	PlayFX(); // Play FX
}

void AUTLTarget::PlayFX_Implementation()
{
	TargetMesh->SetMaterial(0, MaterialsList.Find(TargetState)->LoadSynchronous());
}

#pragma endregion
