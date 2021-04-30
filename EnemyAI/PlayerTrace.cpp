// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTrace.h"

// Sets default values
APlayerTrace::APlayerTrace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void APlayerTrace::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle OutHandle;
	GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &APlayerTrace::DestroyThis, lifetime);
}

void APlayerTrace::DestroyThis()
{
	Destroy();
}

// Called every frame
void APlayerTrace::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

void APlayerTrace::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->GetName().Contains(FString("EnemyCharacter")))
		Destroy();
}