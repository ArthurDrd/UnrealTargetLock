// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Components/UTLTargetLockComponent.h"

#include "GameplayTagContainer.h"
#include "Actors/UTLTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UnrealTargetLock/UnrealTargetLock.h"

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

	// print authority
	if (!GetOwner()->HasAuthority())
	{
		UE_LOG(LogArthur, Warning, TEXT("Has No Authority"));
	}
	else
	{
		UE_LOG(LogArthur, Warning, TEXT("Has Authority"));
	}

	TArray<AActor*> ListOfTargets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AUTLTarget::StaticClass(),  ListOfTargets );

	for (AActor* Target : ListOfTargets)
	{
		AUTLTarget* TargetActor = Cast<AUTLTarget>(Target);
		if(TargetActor == nullptr) return;

		if (TargetActor->GetTags().HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Target.Locked"))))
		{
			TargetActor->Lock(false);
			CurrentTarget = nullptr;
		}
		else
		{
			TargetActor->Lock(true);
			CurrentTarget = TargetActor;
		}
	}
}

void UUTLTargetLockComponent::Server_TargetLock_Implementation()
{
	TargetLock();
}

#pragma endregion