// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "BeastTriggerBox.generated.h"

class AEnemyAIController;

/**
 * 
 */
UCLASS()
class GRIMGRIN_API ABeastTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ABeastTriggerBox();
	
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	AEnemyAIController* Beast = nullptr;
};
