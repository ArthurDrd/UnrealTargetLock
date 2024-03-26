// Copyright (c) 2024, Arthur Durand. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UTLTarget.generated.h"

UCLASS()
class UNREALTARGETLOCK_API AUTLTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUTLTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
