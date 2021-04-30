// Fill out your copyright notice in the Description page of Project Settings.


#include "BTRotateToFindPlayer.h"

#include "EnemyAIController.h"

#define print(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, text);

void UBTRotateToFindPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	elapsedTime += DeltaSeconds;
	//print(FString::SanitizeFloat(elapsedTime));

	if (elapsedTime < time)
	{
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

		if (EnemyAIController)
			EnemyAIController->Rotate(3.14f * 2.f / time);

		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
	else
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTRotateToFindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bNotifyTick = 1;

	elapsedTime = 0.f;

	return EBTNodeResult::InProgress;
}