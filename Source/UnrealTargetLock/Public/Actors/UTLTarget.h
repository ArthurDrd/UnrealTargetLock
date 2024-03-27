// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "UTLTarget.generated.h"

class UCapsuleComponent;

UCLASS()
class UNREALTARGETLOCK_API AUTLTarget : public AActor
{
	GENERATED_BODY()

#pragma region Base Functions

//////////////////////////////////////////////////////////////////////////
/// Base Functions
//////////////////////////////////////////////////////////////////////////
	
public:	
	// Sets default values for this actor's properties
	AUTLTarget();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma endregion

#pragma region Base Properties

//////////////////////////////////////////////////////////////////////////
/// Base Properties
//////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UCapsuleComponent* TargetCapsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* TargetMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tag)
	FGameplayTagContainer TagsContainer;
	
#pragma endregion

#pragma region Target Lock

//////////////////////////////////////////////////////////////////////////
/// Target Lock
//////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION(BlueprintCallable, Category = Lock)
	bool IsLocked() const { return bIsLocked; }
	
	UFUNCTION(BlueprintCallable, Category = Lock)
	void SetLocked(bool bLocked);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = Lock)
	void Server_SetLocked(bool bLocked);
	
protected:
	
	UPROPERTY(ReplicatedUsing=OnRep_IsLocked,EditDefaultsOnly,BlueprintReadOnly, Category = Lock)
	bool bIsLocked = false;

	UFUNCTION()
	void OnRep_IsLocked();

	UFUNCTION(Server, NetMulticast, Unreliable)
	void MulticastOnRep_IsLocked(AActor* IgnoredActor);
	
#pragma endregion 
};
