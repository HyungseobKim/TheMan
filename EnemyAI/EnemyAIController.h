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

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRIMGRIN_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	class UBlackboardComponent* BlackboardComp;

	class UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName PlayerKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName TraceKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName ChasingTimerKey;

	float ChasingTime;
	bool JustLosePlayer = false;

	TArray<AActor*> PatrolPoints;

	virtual void OnPossess(APawn* InPawn) override;
	
public:
	AEnemyAIController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetPlayerSeen(AActor* InActor);

	void SetPlayerHeard(AActor* InActor);

	void SetPlayerNotSeen();
	void SetLosePlayer();
	void ClearTrace();
	void Rotate(float amount);

	void OnEnemyHit(AActor* OtherActor);

	void StartBehave();

	void SetChasingTime(float time);
	

	// Inline getter methods
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }
	
	FORCEINLINE FName GetTargetKey() const { return PlayerKey; }
	FORCEINLINE FName GetLocationKey() const { return LocationKey; }
	FORCEINLINE UObject* GetTarget() const { return BlackboardComp->GetValueAsObject(PlayerKey); }

	int32 CurrentPatrolPoint = 0;

private:
	int32 FindClosestPoint(const FVector& position) const;
};
