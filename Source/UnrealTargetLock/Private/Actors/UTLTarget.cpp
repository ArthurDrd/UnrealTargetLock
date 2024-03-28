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
	
	DOREPLIFETIME(ThisClass, ListOfTags);
}

#pragma endregion

#pragma region Target Lock

//////////////////////////////////////////////////////////////////////////
/// Target Lock
//////////////////////////////////////////////////////////////////////////


void AUTLTarget::Lock(bool bShouldLock)
{
	if(bShouldLock)
		ListOfTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Target.Locked")));
	else
		ListOfTags.RemoveTag(FGameplayTag::RequestGameplayTag(TEXT("Target.Locked")));

	OnRep_TagChanged(); // Call OnRep manually for the server
}

void AUTLTarget::OnRep_TagChanged()
{
	PlayFX(); // Play FX
}

void AUTLTarget::PlayFX_Implementation()
{
	if(ListOfTags.HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Target.Locked"))))
		TargetMesh->SetMaterial(0, MaterialsList.Find("Locked")->LoadSynchronous());
	else
		TargetMesh->SetMaterial(0, MaterialsList.Find("Unlocked")->LoadSynchronous());
}

#pragma endregion
