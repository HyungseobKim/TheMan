//------------------------------------------------------------------------------
//
// File Name:    ShadowPawnMovementComponent.h
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ShadowPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class GRIMGRIN_API UShadowPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 200.f;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
