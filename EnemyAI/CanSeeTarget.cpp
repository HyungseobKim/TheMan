//------------------------------------------------------------------------------
//
// File Name:    CanSeeTarget.cpp
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#include "CanSeeTarget.h"

#include "EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"

#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include <limits> // numeric_limits

#define print(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, text);

void UCanSeeTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!EnemyAIController) return;

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(EnemyAIController->GetCharacter());
	if (!EnemyCharacter) return;

	UObject* Target = EnemyAIController->GetTarget();
	if (Target && !EnemyCharacter->PawnSensingComp->CouldSeePawn(Cast<APawn>(Target)))
	{
		EnemyAIController->SetPlayerNotSeen();
	}
}