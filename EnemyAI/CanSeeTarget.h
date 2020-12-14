//------------------------------------------------------------------------------
//
// File Name:    CanSeeTarget.h
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class GRIMGRIN_API UCanSeeTarget : public UBTService
{
	GENERATED_BODY()

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
