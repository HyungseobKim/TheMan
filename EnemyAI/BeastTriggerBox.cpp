// Fill out your copyright notice in the Description page of Project Settings.


#include "BeastTriggerBox.h"
#include "EnemyAIController.h"

ABeastTriggerBox::ABeastTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ABeastTriggerBox::OnOverlapBegin);
}

void ABeastTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->GetName().Contains(FString("ThirdPerson")))
		{
			Beast->StartBehave();
			Destroy();
		}
	}
}