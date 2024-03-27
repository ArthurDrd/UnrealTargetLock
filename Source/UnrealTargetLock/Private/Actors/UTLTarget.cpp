// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Actors/UTLTarget.h"

#include "Components/CapsuleComponent.h"
#include "GameplayTags/UTLGameplayTags.h"
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
	
	DOREPLIFETIME(ThisClass, bIsLocked);
}

#pragma endregion

#pragma region Target Lock

//////////////////////////////////////////////////////////////////////////
/// Target Lock
//////////////////////////////////////////////////////////////////////////

void AUTLTarget::SetLocked(bool bLocked)
{
	if(!HasAuthority())
		Server_SetLocked(bLocked);
	
	bIsLocked = bLocked;
	OnRep_IsLocked(); // Call OnRep manually for the server
}


void AUTLTarget::Server_SetLocked_Implementation(bool bLocked)
{
	SetLocked(bLocked);
}

void AUTLTarget::OnRep_IsLocked()
{
	// Local FX
	
	MulticastOnRep_IsLocked(this); // FX for all other clients
}

void AUTLTarget::MulticastOnRep_IsLocked_Implementation(AActor* IgnoredActor)
{
	if (this == IgnoredActor)
		return;

	UE_LOG(LogNet, Log, TEXT("MulticastOnRep_IsLocked_Implementation"));
}

#pragma endregion