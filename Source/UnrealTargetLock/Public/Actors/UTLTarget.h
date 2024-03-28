// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "UTLTarget.generated.h"

class UCapsuleComponent; // Capsule Component forward declaration

UENUM()
enum class ETargetState : uint8
{
	Default,
	Targeted
};

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
	ETargetState GetState() const { return TargetState; }
	
	UFUNCTION(BlueprintCallable, Category = Lock)
	void SetState(const ETargetState NewState);
	
protected:
	
	UPROPERTY(ReplicatedUsing=OnRep_StateChanged,EditDefaultsOnly,BlueprintReadOnly, Category = Lock)
	ETargetState TargetState = ETargetState::Default;

	UFUNCTION()
	void OnRep_StateChanged();

	UFUNCTION(BlueprintNativeEvent, Category = Lock)
	void PlayFX();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lock)
	TMap<ETargetState, TSoftObjectPtr<UMaterialInterface>> MaterialsList;
	
#pragma endregion 
};
