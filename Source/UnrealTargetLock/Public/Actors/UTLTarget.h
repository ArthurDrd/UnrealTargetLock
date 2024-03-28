// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "UTLTarget.generated.h"

class UCapsuleComponent; // Capsule Component forward declaration

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
	
#pragma endregion

#pragma region Target Lock

//////////////////////////////////////////////////////////////////////////
/// Target Lock
//////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION(BlueprintCallable, Category = Lock)
	FGameplayTagContainer GetTags() const { return ListOfTags; }
	
	UFUNCTION(BlueprintCallable, Category = Lock)
	void Lock(bool bShouldLock);
	
protected:
	
	UPROPERTY(ReplicatedUsing=OnRep_TagChanged, EditDefaultsOnly, BlueprintReadOnly, Category = "Lock|Tags")
	FGameplayTagContainer ListOfTags;

	UFUNCTION()
	void OnRep_TagChanged();

	UFUNCTION(BlueprintNativeEvent, Category = "Lock")
	void PlayFX();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock|Materials")
	TMap<FName, TSoftObjectPtr<UMaterialInterface>> MaterialsList;
	
#pragma endregion 
};
