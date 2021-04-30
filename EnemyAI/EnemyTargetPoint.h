//------------------------------------------------------------------------------
//
// File Name:    EnemyTargetPoint.h
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright ?2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemyTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class GRIMGRIN_API AEnemyTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BeastAI")
	float WaitingTime = 0.f;
};
