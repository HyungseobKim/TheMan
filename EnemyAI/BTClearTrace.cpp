// Fill out your copyright notice in the Description page of Project Settings.


#include "BTClearTrace.h"

#include "EnemyAIController.h"

EBTNodeResult::Type UBTClearTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController)
	{
		EnemyAIController->ClearTrace();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}