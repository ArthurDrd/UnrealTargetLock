// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Actors/UTLTarget.h"

#include "Components/CapsuleComponent.h"
#include "GameplayTags/UTLGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "UnrealTargetLock/UnrealTargetLock.h"

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
	
	TagsContainer.AddTag(TAG_Target_Lockable);
}

// Called when the game starts or when spawned
void AUTLTarget::BeginPlay()
{
	Super::BeginPlay();
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
	UE_LOG(LogArthur, Warning, TEXT("OnStateChanged"));
	
	PlayFX(); // Play FX
}

void AUTLTarget::PlayFX_Implementation()
{
	TargetMesh->SetMaterial(0, MaterialsList.Find(TargetState)->Get());
}

#pragma endregion
