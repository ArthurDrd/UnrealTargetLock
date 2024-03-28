// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Components/UTLTargetLockComponent.h"

#include "GameplayTagContainer.h"
#include "Actors/UTLTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#pragma region Base Functions

//////////////////////////////////////////////////////////////////////////
/// Base Functions
//////////////////////////////////////////////////////////////////////////

// Sets default values for this component's properties
UUTLTargetLockComponent::UUTLTargetLockComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UUTLTargetLockComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, CurrentTarget);
}

// Called when the game starts
void UUTLTargetLockComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(TargetLockAction != nullptr, TEXT("TargetLockAction is nullptr"))
}

#pragma endregion

#pragma region Target Lock

//////////////////////////////////////////////////////////////////////////
/// Target Lock
//////////////////////////////////////////////////////////////////////////

void UUTLTargetLockComponent::TargetLock()
{
	if(!GetOwner()->HasAuthority())
	{
		Server_TargetLock();
		return;
	}

	if (CurrentTarget != nullptr)
	{
		CurrentTarget->Lock(false);
		CurrentTarget = nullptr;
		return;
	}
	
	TArray<AActor*> ListOfTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AUTLTarget::StaticClass(),  ListOfTargets );

	for (AActor* Target : ListOfTargets)
	{
		AUTLTarget* TargetActor = Cast<AUTLTarget>(Target);
		if(TargetActor == nullptr) return;

		if (!TargetActor->GetTags().HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Target.Locked"))) && TargetActor->GetTags().HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Target.Lockable"))))
		{
			TargetActor->Lock(true);
			CurrentTarget = TargetActor;
			return;
		}
	}
}

void UUTLTargetLockComponent::Server_TargetLock_Implementation()
{
	TargetLock();
}

#pragma endregion