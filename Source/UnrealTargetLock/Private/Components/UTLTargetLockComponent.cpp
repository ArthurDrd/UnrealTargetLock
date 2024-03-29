// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Components/UTLTargetLockComponent.h"

#include "GameplayTagContainer.h"
#include "Actors/UTLTarget.h"
#include "Camera/CameraComponent.h"
#include "Components/UTLHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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

void UUTLTargetLockComponent::TargetDeath(AActor* DeadActor)
{
	if(CurrentTarget == DeadActor)
	{
		CurrentTarget = nullptr;
	}
}

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
		CurrentTarget->GetHealthComponent()->OnDeathDelegate.RemoveDynamic(this, &UUTLTargetLockComponent::TargetDeath);
		CurrentTarget = nullptr;
		return;
	}

	// List of actor hit by the raycast
	TArray<FHitResult> OutHits;

	// Properties of the raycast
	FVector Start;
	FVector End;
	FVector LookForwardVector;

	const auto OwnersComponent = GetOwner()->GetComponentByClass(UCameraComponent::StaticClass());
	if(OwnersComponent)
	{
		const auto* OwnersCameraComponent = Cast<UCameraComponent>(OwnersComponent);
		Start = OwnersCameraComponent->GetComponentLocation();
		LookForwardVector = UKismetMathLibrary::GetForwardVector(OwnersCameraComponent->GetComponentRotation());
	}
	else
	{
		Start = GetOwner()->GetActorLocation();
		LookForwardVector = UKismetMathLibrary::GetForwardVector(GetOwner()->GetActorRotation());
	}

	Start += LookForwardVector * LockOffset;
	
	End = Start + LookForwardVector * LockForwardDistance;

	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(GetOwner());
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(LockPrecision);

	#if !UE_BUILD_SHIPPING
	
	if(bShouldDebugTrace)
	{
		TArray<FHitResult> GarbageOutHits;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, LockPrecision,
			TArray<TEnumAsByte<EObjectTypeQuery>>{EObjectTypeQuery::ObjectTypeQuery1}, false,
			TArray<AActor*>{GetOwner()}, EDrawDebugTrace::ForDuration, GarbageOutHits, true,
			FLinearColor::Red, FLinearColor::Green, DebugTraceDuration);
	}

	#endif
	
	GetWorld()->SweepMultiByChannel(OutHits,Start,End,FQuat::Identity,LockCollisionChannel,CollisionShape,CollisionParameters);
	
	for (const FHitResult& Hit : OutHits)
	{
		AUTLTarget* TargetActor = Cast<AUTLTarget>(Hit.GetActor());
		if(TargetActor == nullptr) return;
		
		if (!TargetActor->GetTags().HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Target.Locked"))) && TargetActor->GetTags().HasTagExact(FGameplayTag::RequestGameplayTag(TEXT("Target.Lockable"))))
		{
			TargetActor->Lock(true);
			CurrentTarget = TargetActor;
			CurrentTarget->GetHealthComponent()->OnDeathDelegate.AddDynamic(this, &UUTLTargetLockComponent::TargetDeath);
			return;
		}
	}
}

void UUTLTargetLockComponent::Server_TargetLock_Implementation()
{
	TargetLock();
}

#pragma endregion