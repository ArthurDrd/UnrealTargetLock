// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTLHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageTakenSignature, AActor*, HitActor, int32, DamageAmount);

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma endregion

#pragma region Health
	
//////////////////////////////////////////////////////////////////////////
/// Health
//////////////////////////////////////////////////////////////////////////

public:
	
	/**
	 * @brief Get the health of the actor
	 * @return Health of the actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetHealth() const { return Health; }

	/**
	 * @brief Get the maximum health of the actor
	 * @return Maximum health of the actor
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetMaxHealth() const { return MaxHealth; }
	
protected:

	/**
	 * @brief Set the health of the actor
	 * @param NewHealth: New health value
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(int32 NewHealth);

	/**
	 * @brief Set the health of the actor
	 * @param NewMaxHealth: New max health value
	 */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(int32 NewMaxHealth);

	UFUNCTION(Server, Reliable, Category = "Health")
	void Server_SetMaxHealth(int32 NewMaxHealth);
	
private:
	
	/**
	 * @brief Health of the actor
	 */
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	int32 Health = 100;
	
	/**
	 * @brief Maximum health of the actor
	 */
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Health")
	int32 MaxHealth = 100;

	/**
	 * @brief Replication of the health
	 * @param OldHealth: Old health value
	 */
	UFUNCTION()
	void OnRep_Health(int32 OldHealth);


#pragma endregion

#pragma region damage
	
//////////////////////////////////////////////////////////////////////////
/// Damage
//////////////////////////////////////////////////////////////////////////

public:
	
	/**
	 * @brief Take damage
	 * @param DamageAmount: Amount of damage to take
	 */
	UFUNCTION(BlueprintCallable, Category = "Health|Damage")
	void TakeDamage(int32 DamageAmount);

	UFUNCTION(Server, Reliable, WithValidation, Category = "Health|Damage")
	void Server_TakeDamage(int32 DamageAmount);

#pragma endregion

#pragma region events
	
//////////////////////////////////////////////////////////////////////////
/// Events
//////////////////////////////////////////////////////////////////////////

public:

	/**
	 * @brief Event called when the actor dies
	 */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeathSignature OnDeathDelegate;

	/**
	 * @brief Event called when the actor takes damage
	 */
	UPROPERTY(BlueprintAssignable, Category = "Health|Damage")
	FOnDamageTakenSignature OnDamageTakenDelegate;

#pragma endregion

};