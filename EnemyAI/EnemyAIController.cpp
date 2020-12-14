//------------------------------------------------------------------------------
//
// File Name:    EnemyAIController.h
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Kismet/GameplayStatics.h"

#include "EnemyCharacter.h"
#include "EnemyTargetPoint.h"

#include "Spawnpoint.h"

#define print(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, text);
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT(text), fstring));

AEnemyAIController::AEnemyAIController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	PlayerKey = "Target";
	LocationKey = "LocationToGo";
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn);

	if (EnemyCharacter && EnemyCharacter->BehaviorTree)
	{
		BlackboardComp->InitializeBlackboard(*(EnemyCharacter->BehaviorTree->BlackboardAsset));

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyTargetPoint::StaticClass(), PatrolPoints);
		BlackboardComp->SetValueAsObject(LocationKey, Cast<AEnemyTargetPoint>(PatrolPoints[0]));

		BehaviorComp->StartTree(*EnemyCharacter->BehaviorTree);
	}
}

void AEnemyAIController::SetPlayerSeen(APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerKey, InPawn);
	}
}

void AEnemyAIController::SetPlayerNotSeen()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetCharacter());
	if (!EnemyCharacter) return;

	BlackboardComp->ClearValue(PlayerKey);

	int32 closestPoint = FindClosestPoint(EnemyCharacter->GetActorLocation());
	BlackboardComp->SetValueAsObject(LocationKey, PatrolPoints[closestPoint]);

	EnemyCharacter->SetBaseMaterial();
}

void AEnemyAIController::OnEnemyHit(AActor* OtherActor)
{
	UObject* Player = BlackboardComp->GetValueAsObject(PlayerKey);

	if (Player && Player == Cast<APawn>(OtherActor))
	{
		//printFString("OtherActor: %s", *OtherActor->GetName());
		//print("You have been caught!");

		((USpawnpoint*)(((APlayerController*)Player)->GetComponentByClass(USpawnpoint::StaticClass())))->Respawn();
	}
}

int32 AEnemyAIController::FindClosestPoint(const FVector& position) const
{
	int32 index = 0;
	float closest = std::numeric_limits<float>::max();

	for (int32 i = 0; i < PatrolPoints.Num(); ++i)
	{
		const FVector& pos = PatrolPoints[i]->GetActorLocation();
		float distance = FVector::Dist(position, pos);
		if (distance < closest)
		{
			index = i;
			closest = distance;
		}
	}

	return index;
}