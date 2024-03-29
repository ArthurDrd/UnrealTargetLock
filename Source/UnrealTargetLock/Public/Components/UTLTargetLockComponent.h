// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UTLTargetLockComponent.generated.h"

class AUTLTarget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNREALTARGETLOCK_API UUTLTargetLockComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region Base Functions

//////////////////////////////////////////////////////////////////////////
/// Base Functions
//////////////////////////////////////////////////////////////////////////
	
public:
	// Sets default values for this component's properties
	UUTLTargetLockComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

#pragma endregion
	
#pragma region Target Lock

//////////////////////////////////////////////////////////////////////////
/// Target Lock
//////////////////////////////////////////////////////////////////////////

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	class UInputAction* TargetLockAction;
	
	UFUNCTION(BlueprintCallable, Category="Lock")
	AUTLTarget* GetCurrentTarget() const { return CurrentTarget; }

	UFUNCTION()
	void TargetDeath(AActor* DeadActor);
	
	UFUNCTION(BlueprintCallable, Category="Lock")
	void TargetLock();

protected:

	// Target Lock
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lock")
	int32 LockForwardDistance = 2000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lock")
	int32 LockPrecision = 300;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lock")
	int32 LockOffset = 250;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lock")
	TEnumAsByte<ECollisionChannel> LockCollisionChannel = ECC_Visibility;

	// Debug
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lock|Debug")
	bool bShouldDebugTrace = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Lock|Debug")
	int32 DebugTraceDuration = 5;
	
	
private:

	UPROPERTY(Replicated)
	AUTLTarget* CurrentTarget = nullptr;
	
	UFUNCTION(Server, Reliable, Category="Lock")
	void Server_TargetLock();

#pragma endregion
};
