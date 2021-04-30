// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTRotateToFindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GRIMGRIN_API UBTRotateToFindPlayer : public UBTTaskNode
{
	GENERATED_BODY()

	float elapsedTime = 0.f;
	const float time = 3.f;
	
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
