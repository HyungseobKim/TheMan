//------------------------------------------------------------------------------
//
// File Name:    EnemyAIController.h
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright ?2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#include "EnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerBox.h"

#include "EnemyCharacter.h"
#include "EnemyTargetPoint.h"

#include "BeastTriggerBox.h"

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
	TraceKey = "Trace";
	ChasingTimerKey = "ChasingTimer";
}

// Will be called in AEnemyCharacter::BeginPlay()
void AEnemyAIController::SetChasingTime(float time)
{
	ChasingTime = time;
	BlackboardComp->SetValueAsFloat(ChasingTimerKey, 0.f);
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (JustLosePlayer == true)
	{
		float ChasingTimer = BlackboardComp->GetValueAsFloat(ChasingTimerKey);
		if (ChasingTimer > 0.f)
			BlackboardComp->SetValueAsFloat(ChasingTimerKey, ChasingTimer - DeltaTime);
		else
		{
			JustLosePlayer = false;
			SetLosePlayer();
		}
	}
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

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABeastTriggerBox::StaticClass(), FoundActors);

		if (FoundActors.Num() == 0)
			StartBehave();
		else
		{
			for (auto box : FoundActors)
				Cast<ABeastTriggerBox>(box)->Beast = this;
		}
	}
}

void AEnemyAIController::StartBehave()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetCharacter());
	BehaviorComp->StartTree(*EnemyCharacter->BehaviorTree);
}

void AEnemyAIController::SetPlayerSeen(AActor* InActor)
{
	JustLosePlayer = false;

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PlayerKey, InActor);
		BlackboardComp->SetValueAsVector(TraceKey, InActor->GetActorLocation());
		BlackboardComp->SetValueAsFloat(ChasingTimerKey, 0.0f);
	}
}

void AEnemyAIController::SetPlayerHeard(AActor* InActor)
{
	if (BlackboardComp)
		BlackboardComp->SetValueAsVector(TraceKey, InActor->GetActorLocation());
}

void AEnemyAIController::SetPlayerNotSeen()
{
	// It already lost player
	if (JustLosePlayer == true || BlackboardComp->GetValueAsFloat(ChasingTimerKey) > 0.0f)
		return;

	JustLosePlayer = true;
	BlackboardComp->SetValueAsFloat(ChasingTimerKey, ChasingTime);
}

void AEnemyAIController::SetLosePlayer()
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetCharacter());
	if (!EnemyCharacter) return;

	// Must be before clear player key.
	//EnemyCharacter->SpawnPlayerTrace(Cast<APawn>(BlackboardComp->GetValueAsObject(PlayerKey)));
	BlackboardComp->ClearValue(PlayerKey);

	AEnemyTargetPoint* closestPoint = Cast<AEnemyTargetPoint>(PatrolPoints[FindClosestPoint(EnemyCharacter->GetActorLocation())]);
	BlackboardComp->SetValueAsObject(LocationKey, closestPoint);
	BlackboardComp->SetValueAsFloat("WaitingTime", closestPoint->WaitingTime);

	EnemyCharacter->SetBaseMaterial();
}

void AEnemyAIController::ClearTrace()
{
	BlackboardComp->ClearValue(TraceKey);
}

void AEnemyAIController::Rotate(float amount)
{
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetCharacter());

	if (EnemyCharacter)
	{
		FRotator NewRotation = EnemyCharacter->GetActorRotation();
		NewRotation.Yaw += amount;

		EnemyCharacter->SetActorRotation(NewRotation);
	}
}

void AEnemyAIController::OnEnemyHit(AActor* OtherActor)
{
	UObject* Player = BlackboardComp->GetValueAsObject(PlayerKey);

	if (Player && Player == Cast<APawn>(OtherActor))
	{
		//printFString("OtherActor: %s", *OtherActor->GetName());
		//print("You have been caught!");

		AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetCharacter());
		EnemyCharacter->CaughtPlayer(); //Unreal event

		((USpawnpoint*)(((APlayerController*)Player)->GetComponentByClass(USpawnpoint::StaticClass())))->BlackFadeRespawn(1.0f);
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