//------------------------------------------------------------------------------
//
// File Name:    BTSelectPatrolPoint.cpp
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#include "BTSelectPatrolPoint.h"

#include "EnemyTargetPoint.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (EnemyAIController)
	{
		UBlackboardComponent* BlackboardComp = EnemyAIController->GetBlackboardComp();

		AEnemyTargetPoint* CurrentPoint = Cast<AEnemyTargetPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));

		TArray<AActor*> AvailablePatrolPoints = EnemyAIController->GetPatrolPoints();

		AEnemyTargetPoint* NextPatrolPoint = nullptr;

		if (EnemyAIController->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1)
		{
			NextPatrolPoint = Cast<AEnemyTargetPoint>(AvailablePatrolPoints[++EnemyAIController->CurrentPatrolPoint]);
		}
		else // At the last point
		{
			NextPatrolPoint = Cast<AEnemyTargetPoint>(AvailablePatrolPoints[0]);
			EnemyAIController->CurrentPatrolPoint = 0;
		}

		BlackboardComp->SetValueAsObject("LocationToGo", NextPatrolPoint);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}