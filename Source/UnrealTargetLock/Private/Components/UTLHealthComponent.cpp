// Copyright (c) 2024, Arthur Durand. All rights reserved.


#include "Components/UTLHealthComponent.h"

#include "Net/UnrealNetwork.h"

#pragma region Base Functions

//////////////////////////////////////////////////////////////////////////
/// Base Functions
//////////////////////////////////////////////////////////////////////////

// Sets default values for this component's properties
UUTLHealthComponent::UUTLHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UUTLHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Health);
	DOREPLIFETIME(ThisClass, MaxHealth);
}


// Called when the game starts
void UUTLHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

#pragma endregion

#pragma region Health

//////////////////////////////////////////////////////////////////////////
/// Health
//////////////////////////////////////////////////////////////////////////

void UUTLHealthComponent::SetHealth(int32 NewHealth)
{
	const int32 OldHealth = Health;
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	
	OnRep_Health(OldHealth); // Call OnRep manually for the server
}

void UUTLHealthComponent::SetMaxHealth(int32 NewMaxHealth)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_SetMaxHealth(NewMaxHealth);
		return;
	}
	
	MaxHealth = NewMaxHealth;
	SetHealth(MaxHealth);
}

void UUTLHealthComponent::Server_SetMaxHealth_Implementation(int32 NewMaxHealth)
{
	SetMaxHealth(NewMaxHealth);
}

void UUTLHealthComponent::OnRep_Health(int32 OldHealth)
{
	if (OldHealth > Health)
	{
		OnDamageTakenDelegate.Broadcast(this->GetOwner(), OldHealth - Health);
	}
}

#pragma endregion

#pragma region damage
	
//////////////////////////////////////////////////////////////////////////
/// Damage
//////////////////////////////////////////////////////////////////////////


void UUTLHealthComponent::TakeDamage(int32 DamageAmount)
{
	if (!GetOwner()->HasAuthority())
	{
		Server_TakeDamage(DamageAmount);
		return;
	}
	
	SetHealth(Health - DamageAmount);

	if (Health <= 0)
	{
		OnDeathDelegate.Broadcast(this->GetOwner());
		GetOwner()->Destroy();
	}
}

void UUTLHealthComponent::Server_TakeDamage_Implementation(int32 DamageAmount)
{
	TakeDamage(	DamageAmount);
}

bool UUTLHealthComponent::Server_TakeDamage_Validate(int32 DamageAmount)
{
	return (DamageAmount <= 0 || Health <= 0);
}

#pragma endregion